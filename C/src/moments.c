/* ************************************************************************** */
/** @file moments.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for initialising and calculating the moments of
 *  the radiation field during the MC iterations.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** init_moments
 *
 *  @brief Initialise a Moments_t structure.
 *
 *  @param[in, out] *moments. An uninitialised Moments_t struct.
 *
 *  @return 0
 *
 *  @details
 *
 *  Allocates memory for the pointers in a Moments_t struct with n_levels + 1
 *  elements. The elements of these arrays are initialised to zero. Calloc may
 *  have been a better alternative here than malloc followed by a loop.
 *
 * ************************************************************************** */

void
init_moments(Moments_t *moments)
{
  moments->j_plus = calloc(moments->n_levels + 1, sizeof *moments->j_plus);
  moments->h_plus = calloc(moments->n_levels + 1, sizeof *moments->h_plus);
  moments->k_plus = calloc(moments->n_levels + 1, sizeof *moments->k_plus);
  moments->j_minus = calloc(moments->n_levels + 1, sizeof *moments->j_minus);
  moments->h_minus = calloc(moments->n_levels + 1, sizeof *moments->h_minus);
  moments->k_minus = calloc(moments->n_levels + 1, sizeof *moments->k_minus);
}

/* ************************************************************************** */
/** increment_radiation_moment_estimators
 *
 *  @brief Update the value of the moments of the radiation field.
 *
 *  @param[in,out] *moments  A pointer to an initialised Moments_t structure.
 *  @param[in] z_pre_scat      The original position before a photon has
 *                             been transported a length ds.
 *  @param[in] z_post_scat     The updated position after the photon has
 *                             been transported a length ds.
 *  @param[in] costheta        The cosine of the theta direction of the
 *                             photon.
 *
 *  @details
 *
 *  Calculates the moments of the radiation field at each level specified by
 *  n_levels + 1.
 *
 *      * J - Mean Intensity
 *      * H - Eddington Flux
 *      * K - Radiation Pressure
 *
 *  The function calculates which level the the pre and post scattering
 *  positions corresponds to and increments the count with the relevant
 *  quantity. Upwards and downwards directions are calculated separately, thus
 *  the final value will be the sum of the upwards and downwards direction.
 *
 *  The moments are calculated essentially by photon counters, i.e. it counts
 *  how many times photons pass through this level.
 *
 * ************************************************************************** */

void
increment_radiation_moment_estimators(Moments_t *moments, double z_pre, double z_post, double costheta)
{
  /*
   * If the photon hasn't moved a vast distance, then we don't need to do
   * anything
   */

  if((z_pre > 0) && (z_post > 0) && (((int) (z_pre * moments->n_levels)) == ((int) (z_post * moments->n_levels))))
    return;

  int pre_scat_level_ele;
  int post_scat_level_ele;

  if(costheta > 0)
  {
    if(z_pre <= 0)
    {
      pre_scat_level_ele = 0;
    }
    else
    {
      pre_scat_level_ele = (int) (z_pre * moments->n_levels) + 1;
    }

    if(z_post >= 1)
    {
      post_scat_level_ele = moments->n_levels;
    }
    else
    {
      post_scat_level_ele = (int) (z_post * moments->n_levels);
    }

    for(int i = pre_scat_level_ele; i <= post_scat_level_ele; i++)
    {
      moments->j_plus[i] += 1.0 / costheta;
      moments->h_plus[i] += 1;
      moments->k_plus[i] += costheta;
    }
  }
  else if(costheta < 0)
  {
    pre_scat_level_ele = (int) (z_pre * moments->n_levels);

    if(z_post <= 0)
    {
      post_scat_level_ele = 0;
    }
    else
    {
      post_scat_level_ele = (int) (z_post * moments->n_levels) + 1;
    }

    for(int i = post_scat_level_ele; i <= pre_scat_level_ele; i++)
    {
      moments->j_minus[i] += 1.0 / fabs(costheta);
      moments->h_minus[i] -= 1;
      moments->k_minus[i] += fabs(costheta);
    }
  }
}
