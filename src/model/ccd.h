#ifndef __AR_M_CCD_H__
#define __AR_M_CCD_H__

#include "model.h"

/**
   \brief Performs one Cyclic Coordinate Descent iteration over all
   elements in a limb chain starting at specified point.

   \param goal_pos target position.
   \param chain limb node of a limb chain to be used as root.

   \param epsilon algorithm threshold. If distance between goal and
   end effector position is less than epsilon the algorithm stops.
   \note goal_pos may not be NULL.
   \note goal_pos must be a 3D vector.
   \note chain may not be NULL.
   \note epsilon must be positive.

   \return true if distance between goal and end effector is less than
   epsilon. Otherwise false.
*/
bool
ar_m_ccd_chain_iter(const gsl_vector* goal_pos,
		    struct ar_m_limb* chain,
		    double epsilon);

/**
   \brief Performs Cyclic Coordinate Descent inverse kinematics
   optimization towards given goal position.
   \param goal_pos target end effector position.
   \param chain limb node in limb chain to use as chain root.

   \param epsilon algorithm threshold. If distance between goal and
   end effector position is equal or less than epsilon, the algorithm
   stops.
   \param max_iterations maximum count of iterations to perform.
   \note goal_pos may not be NULL.
   \note goal_pos must be a 3D vector.
   \note chain may not be NULL.
   \note epsilon must be positive.
   \return true if end effector reached goal position with distance
   equal or less than epsilon. Otherwise false.
*/
bool ar_m_ccd(const gsl_vector* goal_pos,
	      struct ar_m_limb *chain,
	      double epsilon,
	      size_t max_iterations);



#endif
