#include "ccd.h"
#include <math.h>
#include "gsl_vector_helpers.h"
#include <gsl/gsl_blas.h>
#include <assert.h>

#define ACOS_NUM_ERROR_EPSILON 0.000001

bool
ar_m_ccd_chain_iter(const gsl_vector* goal_pos,
		    struct ar_m_limb* chain,
		    double epsilon)
{
  assert(goal_pos != NULL);
  assert(goal_pos->size == 3);
  assert(chain != NULL);
  assert(epsilon > 0);

  size_t chain_len = ar_m_limb_chain_length(chain);
  int root_index;
  gsl_matrix *end_effector_dh, *root_dh;
  gsl_vector *end_effector_pos, *root_pos;
  gsl_vector *root_to_end_effector = gsl_vector_alloc(3);
  gsl_vector *root_to_goal = gsl_vector_alloc(3);
  gsl_vector *plane_root_to_end_effector = gsl_vector_alloc(3);
  gsl_vector *plane_root_to_goal = gsl_vector_alloc(3);

  bool stop = false;

  // from each chain node we have to rotate the root->end_effector
  // vector towards the root->goal vector.
  for(root_index = chain_len - 1; root_index != -1; --root_index)
    {
      // calculate end effector and root positions
      end_effector_dh = ar_m_limb_chain_trnsfmtn_mtx_all(chain);
      root_dh = ar_m_limb_chain_trnsfmtn_mtx_n(chain, root_index);
      end_effector_pos = ar_m_dh_pos(end_effector_dh);
      root_pos = ar_m_dh_pos(root_dh);

      // if both positions are near enough, we can stop
      double distance = gsl_vector_distance(end_effector_pos, goal_pos);
      if (distance <= epsilon)
	{
	  gsl_vector_free(root_to_end_effector);
	  gsl_vector_free(root_to_goal);
	  gsl_vector_free(plane_root_to_end_effector);
	  gsl_vector_free(plane_root_to_goal);
	  stop = true;
	}
      else // positions are not near enough, optimize
	{
	  // calculate root->end_effector and root->goal vectors
	  // the former has to be rotated towards the latter
	  gsl_vector_memcpy(root_to_end_effector, end_effector_pos);
	  gsl_vector_sub(root_to_end_effector, root_pos);
	  
	  gsl_vector_memcpy(root_to_goal, goal_pos);
	  gsl_vector_sub(root_to_goal, root_pos);
	  
	  // if the root is directly on goal, we cannot do anything
	  // proceed with the next one and prey
	  if (gsl_vector_length_sq(root_to_goal) != 0.0)
	    {	  
	      // coordinate transformation matrix.
	      // both vectors have to be brought into a uniform
	      // coordinate system
	      // where they can be rotated around an axis (here: Z)
	      gsl_matrix_view root_rot_m = gsl_matrix_submatrix(root_dh, 0, 0, 3, 3);
	      gsl_blas_dgemv(CblasNoTrans, 1.0, &root_rot_m.matrix, root_to_end_effector, 0.0, plane_root_to_end_effector);
	      gsl_blas_dgemv(CblasNoTrans, 1.0, &root_rot_m.matrix, root_to_goal, 0.0, plane_root_to_goal);
	      // before computing the angle the vectors have to be
	      // brought on the same plane and normalized.
	      // as they are along z-axis now, simply make both z
	      // components same.
	      gsl_vector_set(plane_root_to_goal, 2, gsl_vector_get(plane_root_to_end_effector, 2));
	      gsl_vector_normalize(plane_root_to_end_effector);
	      gsl_vector_normalize(plane_root_to_goal);
	      
	      // cos(theta) = dot_product(v1, v2)
	      // due to numerical errors this value is sometimes
	      // bigger than 1.0 eliminate this.
	      double cos_dtheta = gsl_vector_dot(plane_root_to_goal, plane_root_to_end_effector);
	      if (cos_dtheta > 1.0 && cos_dtheta - ACOS_NUM_ERROR_EPSILON <= 1.0) // eliminate numerical error 
		cos_dtheta = 1.0;
	      // the rotation angle
	      double dtheta = acos(cos_dtheta);
	      
	      // cross product is needed to determine the rotation
	      // direction.
	      gsl_vector* x = gsl_vector_cross(plane_root_to_goal, plane_root_to_end_effector);
	      double sign = gsl_vector_get(x, 2) < 0 ? 1.0 : -1.0;
	      gsl_vector_free(x);
	      
	      // find the root node and rotate its angle by dtheta.
	      struct ar_m_limb* root = chain;
	      int i;
	      for(i = 0; i != root_index; ++i)
		root = root->next;
	      root->params.theta += sign*dtheta;
	      if (root->params.theta > root->max_theta)
		root->params.theta = root->max_theta;
	      if (root->params.theta < root->min_theta)
		root->params.theta = root->min_theta;
	    }
	  gsl_vector_free(end_effector_pos);
	  gsl_vector_free(root_pos);
	  gsl_matrix_free(end_effector_dh);
	  gsl_matrix_free(root_dh);
	}
      if (stop)
	return true;
    }
  
  return false;
}

bool
ar_m_ccd(const gsl_vector* goal_pos,
	 struct ar_m_limb *chain,
	 double epsilon,
	 size_t max_iterations)
{
  size_t iteration;
  for(iteration = 0; iteration < max_iterations; ++iteration)
    {
      if (ar_m_ccd_chain_iter(goal_pos, chain, epsilon))
	return true;
    }
  return false;
}
