/* ************************************************************************** */
/** @file moments.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for initialising and calculating the moments of
 *  the radiation field during the MC iterations.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** init_jhk
 *
 *  @brief Initialise a JHK_Moments structure.
 *
 *  @param[in, out] JHK_Moments *moments. An uninitialised JHK_Moments struct.
 *
 *  @return 0
 *
 *  @details
 *
 *  Allocates memory for the pointers in a JHK_Moments struct with n_levels + 1
 *  elements. The elements of these arrays are initialised to zero. Calloc may
 *  have been a better alternative here than malloc followed by a loop.
 *
 * ************************************************************************** */

int init_jhk(JHK_Moments *moments)
{
    moments->j_plus = malloc(sizeof(double) * (n_levels + 1));
    moments->h_plus = malloc(sizeof(double) * (n_levels + 1));
    moments->k_plus = malloc(sizeof(double) * (n_levels + 1));
    moments->j_minus = malloc(sizeof(double) * (n_levels + 1));
    moments->h_minus = malloc(sizeof(double) * (n_levels + 1));
    moments->k_minus = malloc(sizeof(double) * (n_levels + 1));

    for (int i = 0; i < n_levels + 1; i++)
    {
        moments->j_plus[i] = 0.0;
        moments->h_plus[i] = 0.0;
        moments->k_plus[i] = 0.0;
        moments->j_minus[i] = 0.0;
        moments->h_minus[i] = 0.0;
        moments->k_minus[i] = 0.0;
    }

    return 0;
}

/* ************************************************************************** */
/** calculate_moments
 *
 *  @brief Update the value of the moments of the radiation field.
 *
 *  @param[in, out] JHK_Moments *moments. A pointer to an initialised
 *  JHK_Moments structure.
 *
 *  @param[in] double z_pre_scat. The original position before a photon has been
 *  transported a length ds.
 *
 *  @param[in] double z_post_scat. The updated position after the photon has
 *  been transported a length ds.
 *
 *  @param[in] double cos_theta. The cosine of the theta direction of the
 *  photon.
 *
 *  @return 0
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

int calculate_moments(JHK_Moments *moments, double z_pre_scat,
    double z_post_scat, double cos_theta)
{
    int pre_scat_level_ele = 0,
        post_scat_level_ele = 0;

    if ( (z_pre_scat > 0) && (z_post_scat > 0) && \
        (((int) (z_pre_scat * n_levels)) == ((int) (z_post_scat * n_levels))))
    {
        return 0;
    }

    if (cos_theta > 0)
    {
        if (z_pre_scat <= 0)
            pre_scat_level_ele = 0;
        else
            pre_scat_level_ele = (int) (z_pre_scat * n_levels) + 1;

        if (z_post_scat >= 1)
            post_scat_level_ele = n_levels;
        else
            post_scat_level_ele = (int) (z_post_scat * n_levels);

        for (int i = pre_scat_level_ele; i <= post_scat_level_ele; i++)
        {
            moments->j_plus[i] += 1.0/cos_theta;
            moments->h_plus[i] += 1;
            moments->k_plus[i] += cos_theta;
        }
    }
    else if (cos_theta < 0)
    {
        pre_scat_level_ele = (int) (z_pre_scat * n_levels);

        if (z_post_scat <= 0)
            post_scat_level_ele = 0;
        else
            post_scat_level_ele = (int) (z_post_scat * n_levels) + 1;

        for (int i = post_scat_level_ele; i <= pre_scat_level_ele; i++)
        {
            moments->j_minus[i] += 1.0/fabs(cos_theta);
            moments->h_minus[i] -= 1;
            moments->k_minus[i] += fabs(cos_theta);
        }
    }

    return 0;
}


