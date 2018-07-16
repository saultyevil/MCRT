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

#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "plane_vars.h"
#include "plane_funcs.h"

gsl_rng *rng;

/* ************************************************************************** */
/** init_gsl_seed
 *
 *  @brief Initialise the random seed for the GSL random number generator.
 *
 *  @param[in] int seed. A seed to use.
 *
 *  @return 0
 *
 *  @details
 *
 *  Initialises the GSL RNG algorithm using the seed provided. The RNG algorithm
 *  choice should be thread safe!!!
 *
 * ************************************************************************** */

int init_gsl_seed(int x_seed)
{
    rng = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(rng, x_seed);

    return 0;
}

/* ************************************************************************** */
/** gsl_rand_num
 *
 *  @brief Return a random number between the boundaries min and max.
 *
 *  @param[in] double min. The minimum value the random number can take.
 *  @param[in] double max. The maximum value the random number can take.
 *
 *  @return A random double between min and max.
 *
 *  @details
 *
 *  This should be thread safe!!!
 *
 * ************************************************************************** */

double gsl_rand_num(double min, double max)
{
    double rand_num = gsl_rng_uniform_pos(rng);

    return min + ((max - min) * rand_num);
}

/* ************************************************************************** */
/** random_tau
 *
 *  @brief Return a random optical depth.
 *
 *  @return a random double which is an optical depth...?
 *
 *  @details
 *
 *  Returns a random optical depeth via -1 * log(1 - rand_num).
 *
 * ************************************************************************** */

double random_tau(void)
{
    return -1.0 * log(1 - gsl_rand_num(0, 1));
}

/* ************************************************************************** */
/** random_cost_phi
 *
 *  @brief Generate a random mu and phi direction.
 *
 *  @param[in, out] double *cos_theta. A pointer for the random mu direction.
 *  @param[in, out] double *phi. A pointer for the random phi direction.
 *
 *  @return 0
 *
 *  @details
 *
 *  Generates a random mu and phi direction usually for use when giving a photon
 *  a new direction after an isotropic scatter.
 *
 * ************************************************************************** */

int random_cost_phi(double *cos_theta, double *phi)
{
    *cos_theta = 2 * gsl_rand_num(0, 1) - 1;
    *phi = 2 * PI * gsl_rand_num(0, 1);

    return 0;
}

/* ************************************************************************** */
/** random_isotropic_direction
 *
 *  @brief Points the photon packet in a new isotropic direction.
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC interation
 *  photon.
 *
 *  @return 0
 *
 *  @details
 *
 *  Points a photon packet in a new direction after an isotropic scattering
 *  event. The random mu and phi directions are generated from the
 *  random_cost_phi function and then a photon's sin and cos terms are updated
 *  to represent the new direction.
 *
 * ************************************************************************** */

int random_isotropic_direction(Photon *packet)
{
    double mu, phi;

    random_cost_phi(&mu, &phi);

    packet->cos_phi = cos(phi);
    packet->sin_phi = sin(phi);
    packet->cos_theta = mu;
    packet->sin_theta = sqrt(1 - mu * mu);

    return 0;
}
