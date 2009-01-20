#include "model.h"
#include "gsl_vector_helpers.h"
#include <math.h>
#include <gsl/gsl_blas.h>
#include <assert.h>

gsl_matrix*
ar_m_dh_trnsfmtn_mtx(struct ar_m_dh_params* dh)
{
  assert(dh != NULL);
  gsl_matrix* r;
  double sin_theta, cos_theta, sin_alpha, cos_alpha;

  sin_theta = sin(dh->theta);
  cos_theta = cos(dh->theta);
  sin_alpha = sin(dh->alpha);
  cos_alpha = cos(dh->alpha);

  r = gsl_matrix_calloc(4,4);

  gsl_matrix_set(r, 0, 0, cos_theta);
  gsl_matrix_set(r, 0, 1, -sin_theta * cos_alpha);
  gsl_matrix_set(r, 0, 2, sin_theta * sin_alpha);
  gsl_matrix_set(r, 0, 3, dh->a * cos_theta);

  gsl_matrix_set(r, 1, 0, sin_theta);
  gsl_matrix_set(r, 1, 1, cos_theta * cos_alpha);
  gsl_matrix_set(r, 1, 2, -cos_theta * sin_alpha);
  gsl_matrix_set(r, 1, 3, dh->a * sin_theta);

  gsl_matrix_set(r, 2, 1, sin_alpha);
  gsl_matrix_set(r, 2, 2, cos_alpha);
  gsl_matrix_set(r, 2, 3, dh->d);

  gsl_matrix_set(r, 3, 3, 1);

  return r;
}

struct ar_m_limb*
ar_m_limb_chain_root(struct ar_m_limb* l)
{
  assert(l != NULL);
  while(l->prev != NULL)
    l = l->prev;
  return l;
}

struct ar_m_limb*
ar_m_limb_chain_end(struct ar_m_limb* l)
{
  assert(l != NULL);
  while(l->next != NULL)
    l = l->next;
  return l;
}

double rad2deg(double rad) { return rad / M_PI * 180.0; }

size_t
ar_m_limb_chain_length(struct ar_m_limb* l)
{
  assert(l != NULL);

  size_t i = 1;
  while(l->next != NULL)
    {
      l = l->next;
      ++i;
    }

  return i;
}

gsl_matrix*
ar_m_limb_chain_trnsfmtn_mtx_n(struct ar_m_limb *l,
			       size_t n)
{
  assert(l != NULL);
  assert(ar_m_limb_chain_length(l) >= n);

  gsl_matrix *r = gsl_matrix_calloc(4,4);
  gsl_matrix_set_identity(r);
  gsl_matrix *tmp = gsl_matrix_alloc(r->size1, r->size2);
  size_t i;
  for(i = 0; i != n; ++i, l = l->next)
    {
      gsl_matrix* dh = ar_m_dh_trnsfmtn_mtx(&l->params);
      gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
		     1.0, r, dh, 0.0, tmp);
      gsl_matrix_memcpy(r, tmp);
      gsl_matrix_free(dh);
    }
  gsl_matrix_free(tmp);
  return r;
}

gsl_matrix*
ar_m_limb_chain_trnsfmtn_mtx_all(struct ar_m_limb* l)
{
  return ar_m_limb_chain_trnsfmtn_mtx_n(l, ar_m_limb_chain_length(l));
}

gsl_vector*
ar_m_dh_pos(const gsl_matrix* dh)
{
  gsl_vector* r = gsl_vector_alloc(3);
  gsl_vector_set(r, 0, gsl_matrix_get(dh, 0, 3));
  gsl_vector_set(r, 1, gsl_matrix_get(dh, 1, 3));
  gsl_vector_set(r, 2, gsl_matrix_get(dh, 2, 3));
  
  return r;
}


