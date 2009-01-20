#include "gsl_vector_helpers.h"

#include <assert.h>
#include <math.h>

gsl_vector*
gsl_vector_cross(const gsl_vector* v1,
		 const gsl_vector* v2)
{
  assert(v1 != NULL);
  assert(v2 != NULL);
  assert(v1->size == 3);
  assert(v2->size == 3);

  gsl_vector *r = gsl_vector_alloc(3);
  double x1,y1,z1,x2,y2,z2;

  x1 = gsl_vector_get(v1, 0);
  y1 = gsl_vector_get(v1, 1);
  z1 = gsl_vector_get(v1, 2);
  
  x2 = gsl_vector_get(v2, 0);
  y2 = gsl_vector_get(v2, 1);
  z2 = gsl_vector_get(v2, 2);

  gsl_vector_set(r, 0, y1*z2 - z1*y2);
  gsl_vector_set(r, 1, z1*x2 - x1*z2);
  gsl_vector_set(r, 2, x1*y2 - y1*x2);

  return r;
}

double
gsl_vector_length_sq(const gsl_vector *v)
{
  assert(v != NULL);

  double sum = 0.0;
  size_t i;
  double x;
  for(i = 0; i < v->size; ++i)
    {
      x = gsl_vector_get(v, i);
      sum += x*x;
    }
  return sum;
}

double
gsl_vector_length(const gsl_vector* v)
{
  return sqrt(gsl_vector_length_sq(v));
}

double
gsl_vector_distance_sq(const gsl_vector* v1,
		       const gsl_vector* v2)
{
  assert(v1 != NULL);
  assert(v2 != NULL);
  assert(v1->size == v2->size);
  
  gsl_vector *diff = gsl_vector_alloc(v1->size);
  gsl_vector_memcpy(diff, v1);
  gsl_vector_sub(diff, v2);
  double len = gsl_vector_length_sq(diff);
  gsl_vector_free(diff);
  return len;
}

double
gsl_vector_distance(const gsl_vector* v1,
		    const gsl_vector* v2)
{
  return sqrt(gsl_vector_distance_sq(v1, v2));
}

double gsl_vector_dot(const gsl_vector *v1,
		      const gsl_vector *v2)
{
  assert(v1 != NULL);
  assert(v2 != NULL);
  assert(v1->size == v2->size);

  double sum = 0.0, a, b;
  size_t i;
  for(i = 0; i < v1->size; ++i)
    {
      a = gsl_vector_get(v1, i);
      b = gsl_vector_get(v2, i);
      sum += (a*b);
    }
  return sum;
}

void gsl_vector_normalize(gsl_vector* v)
{
  double len = gsl_vector_length(v);
  gsl_vector_scale(v, 1.0 / len);
}

