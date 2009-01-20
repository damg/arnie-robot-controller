#ifndef __GSL_VECTOR_HELPERS_H__
#define __GSL_VECTOR_HELPERS_H__

#include <gsl/gsl_vector.h>

/**
   \brief Calculates the cross product vector of two vectors.
   \note v1 may not be NULL.
   \note v2 may not be NULL.
   \note v1 must be a 3D vector.
   \note v2 must be a 3D vector.
   \return Newly allocated cross product vector.
*/
gsl_vector*
gsl_vector_cross(const gsl_vector* v1,
		 const gsl_vector* v2);

/**
   \brief Calculates the squared length of a vector.
   \param v vector to use.
   \note v may not be NULL.
*/
double
gsl_vector_length_sq(const gsl_vector *v);

/**
   \brief Calculates the length of a vector.
   \param v vector to use.
   \note v may not be NULL.
*/
double
gsl_vector_length(const gsl_vector* v);


/**
   \brief Calculates squared distance between two vectors.
   
   \param v1 first vector.
   \param v2 second vector.
   \note v1 may not be NULL.
   \note v2 may not be NULL.
   \note v1 and v2 have to have same number of elements.
*/
double
gsl_vector_distance_sq(const gsl_vector* v1,
		       const gsl_vector* v2);
/**
   \brief Computes the distance between two vectors.
   \param v1 first vector.
   \param v2 second vector.
   \note v1 may not be NULL.
   \note v2 may not be NULL.
   \note v1 and v2 have to have the same number of elements.
*/
double
gsl_vector_distance(const gsl_vector* v1,
		    const gsl_vector* v2);

/**
   \brief Computes the dot product of two vectors.
   \param v1 first vector.
   \param v2 second vector.
   \note v1 may not be NULL.
   \note v2 may not be NULL.
   \note v1 and v2 have to have the same number of elements.
*/
double gsl_vector_dot(const gsl_vector *v1,
		      const gsl_vector *v2);

/**
   \Normalizes a vector.
   \param v vector to normalize.
   \note v may not be NULL.
*/
void gsl_vector_normalize(gsl_vector* v);

#endif
