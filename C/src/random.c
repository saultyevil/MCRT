/* ************************************************************************** */
/** @file random.c
 *  @author Edward Parkinson
 *  @author Nick Higginbottom
 *  @date 12 July 2018
 *
 *  @brief Various functions for calculating random values. Contains the
 *  functions to set up and request a random number as well as random parameters
 *  used for the MC iterations.
 *
 *  (I basically stole the GSL initialisation functions from Nick/PYTHON :^)
 *
 * ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include <gsl/gsl_rng.h>

#include "variables.h"
#include "functions.h"

gsl_rng *rng;

/* ************************************************************************** */
/** init_gsl_seed
 *
 *  @brief Initialise the random SEED for the GSL random number generator.
 *
 *  @param[in] seed
 *
 *  @details
 *
 *  Initialises the GSL RNG algorithm using the SEED provided. The RNG algorithm
 *  choice should be thread safe!!!
 *
 * ************************************************************************** */

void
init_gsl_seed(int seed)
{
  rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(rng, seed);
}

/* ************************************************************************** */
/** gsl_rand_num
 *
 *  @brief Return a random number between the boundaries min and max.
 *
 *  @param[in] min  The minimum value the random number can take.
 *  @param[in] max  The maximum value the random number can take.
 *
 *  @return A random double between min and max.
 *
 *  @details
 *
 *  This should be thread safe!!!
 *
 * ************************************************************************** */

double
gsl_rand_num(double min, double max)
{
  double rand_num = gsl_rng_uniform_pos(rng);
  return min + ((max - min) * rand_num);
}

/* ************************************************************************** */
/** random_tau
 *
 *  @brief Return a random optical depth.
 *
 *  @return a random optical depth
 *
 *  @details
 *
 *  Returns a random optical depth via -1 * log(1 - rand_num).
 *
 * ************************************************************************** */

double
random_tau(void)
{
  return -1.0 * log(1 - gsl_rand_num(0, 1));
}

/* ************************************************************************** */
/** random_theta_phi
 *
 *  @brief Generate a random isotropic theta and phi direction.
 *
 *  @param[in,out] *theta A pointer for the random theta direction.
 *  @param[in,out] *phi   A pointer for the random phi direction.
 *
 *  @details
 *
 *  Generates a random theta and phi direction, usually for use when giving a
 *  photon a new direction after an isotropic scatter.
 *
 * ************************************************************************** */

void
random_theta_phi(double *theta, double *phi)
{
  *theta = acos(2 * gsl_rand_num(0, 1) - 1);
  *phi = 2 * PI * gsl_rand_num(0, 1);
}
