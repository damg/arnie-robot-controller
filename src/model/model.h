#ifndef __AR_M_MODEL_H__
#define __AR_M_MODEL_H__

#include <stdbool.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

struct ar_m_dh_params
{
  double theta, alpha, d, a;
};

struct ar_m_limb;

struct ar_m_limb
{
  struct ar_m_limb *prev, *next;
  struct ar_m_dh_params params;
  double min_theta, max_theta;
};

/**
   \brief Calculate a Denavit-Hartenberg transformation matrix.
   \param dh structure holding D-H parameters.
   \note dh may not be NULL.
   \return newly allocated 4x4 D-H matrix.
*/
gsl_matrix*
ar_m_dh_trnsfmtn_mtx(struct ar_m_dh_params* dh);

/**
   \brief Find the first element of a limb chain.
   \param l an element of a chain.
   \note l may not be NULL.
   \return Pointer to the first limb.
*/
struct ar_m_limb*
ar_m_limb_chain_root(struct ar_m_limb* l);

/**
   \brief Finds the last element of a limb chain.
   \param l pointer to an element in the chain.
   \note l may not be NULL.
   \return Pointer to the last element.
*/
struct ar_m_limb*
ar_m_limb_chain_end(struct ar_m_limb* l);

/**
   \brief Convert radians to degrees.
*/
double rad2deg(double rad);

/**
   \brief Calculates the length of a chain starting at given limb
   node.

   \param l starting limb node.
   \note l may not be NULL.
*/
size_t
ar_m_limb_chain_length(struct ar_m_limb* l);

/**
   \brief Computes full transformation of N nodes starting at given
   limb node.

   \param l starting limb node.
   \param n Count of nodes to consider.
   \note l may not be NULL.
   \note Length of the chain starting from l must be >= n
   \return Newly allocated D-H matrix with full transformation.
*/
gsl_matrix*
ar_m_limb_chain_trnsfmtn_mtx_n(struct ar_m_limb *l,
			       size_t n);

/**
   \brief Calculates the complete transformation of limb chain to its
   end.
   \param l Starting limb node.
   \node l may not be NULL.
   \return Newly allocated D-H transformation matrix.
*/
gsl_matrix*
ar_m_limb_chain_trnsfmtn_mtx_all(struct ar_m_limb* l);

/**
   \brief Extracts position vector from a D-H matrix.
   \param dh D-H matrix.
   \return Newly allocated 3D vector with x, y, z coordinates.
*/
gsl_vector*
ar_m_dh_pos(const gsl_matrix* dh);

#endif
