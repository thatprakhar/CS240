/* Name, hw4.c, CS 24000, Fall 2020
 * Last updated February 5, 2020
 */

/* Add any includes here */
#include "hw4.h"

#include <math.h>


#define MAX_MOD (4.0)

/* Define any global variables here */




/* Define your functions here */

/*
 * This functions returns
 * the sum of two
 * complex numbers
 */

complex_t add_complex(complex_t one, complex_t two) {
  complex_t result = {0.0, 0.0};
  result.x = one.x + two.x;
  result.y = one.y + two.y;
  return result;
} /* add_complex() */

/*
 * This functions returns
 * the negative of a
 * complex number
 */

complex_t neg_complex(complex_t complex_number) {
  complex_t neg_complex_number = {0, 0};
  neg_complex_number.x = -1 * complex_number.x;
  neg_complex_number.y = -1 * complex_number.y;
  return neg_complex_number;
} /* neg_complex() */

/*
 * This functions returns
 * the difference of two
 * complex numbers
 */

complex_t sub_complex(complex_t one, complex_t two) {
  return add_complex(one, neg_complex(two));
} /* sub_complex() */


/*
 * This functions returns
 * the product of two
 * complex numbers
 */

double dot_complex(complex_t one, complex_t two) {
  return ((one.x * two.x) + (one.y * two.y));
} /* dot_complex() */

/*
 * This functions returns
 * the inverse of a
 * complex number
 */

complex_t inv_complex(complex_t complex_number) {
  complex_t inverse = {0, 0};
  double mod = (complex_number.x * complex_number.x)
  + (complex_number.y * complex_number.y);

  if (mod != 0) {
    inverse.x = complex_number.x / mod;
    inverse.y = -complex_number.y / mod;
  } else {
    inverse.x = 0;
    inverse.y = 0;
  }

  return inverse;
} /* inv_complex() */

/*
 * This functions returns
 * the product of two
 * complex numbers
 */

complex_t mul_complex(complex_t one, complex_t two) {
  complex_t product = {0, 0};
  product.x = one.x * two.x - one.y * two.y;
  product.y = one.x * two.y + one.y * two.x;
  return product;
} /* mul_complex() */

/*
 * This functions returns
 * the division of two
 * complex numbers
 */

complex_t div_complex(complex_t one, complex_t two) {
  return mul_complex(one, inv_complex(two));
} /* div_complex() */

/*
 * This functions returns
 * the exponential of a
 * complex number
 */

complex_t exp_complex(complex_t one) {
  complex_t temp = {0, 0};
  double mod = exp(one.x);
  temp.x = mod * cos(one.y);
  temp.y = mod * sin(one.y);
  return temp;
} /* exp_complex() */


/*
 * This functions returns
 * the madelbrot value
 */

int mandelbrot(complex_t one) {
  if ((one.x == 0)
      && (one.y == 0)) {
    return MAX_MANDELBROT;
  }
  complex_t num = {0, 0};

  int num_of_iterations = 0;
  float mod = 0;
  while ((num_of_iterations < MAX_MANDELBROT)
        && (mod < MAX_MOD)) {
    num = mul_complex(num, num);
    num = add_complex(num, one);
    mod = num.x * num.x + num.y * num.y;
    num_of_iterations++;
  }

  if (mod >= MAX_MOD) {
    return num_of_iterations;
  } else {
    return MAX_MANDELBROT;
  }
} /* mandelbrot() */
