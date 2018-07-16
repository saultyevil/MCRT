/* ************************************************************************** */
/** @file intensity.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for creating and initialising the photon escape
 *  angle bins, as well as functions used to bin photons into the correct bin
 *  and calculating the intensity of the binned angles.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** init_photon_hist
 *
 *  @brief Calculate the photon escape angle bin values and initialise all of
 *  the bin counts to 0.
 *
 *  @param[in, out] Mu_hist *hist. A pointer to an uninitialised Mu_hist
 *  struct.
 *
 *  @return 0
 *
 *  @details
 *
 *  Takes in an uninitialised Mu_hist struct, allocates memory for the
 *  pointers within the struct with mu_bin elements and then calculates the
 *  bin's escape angle value and sets the count to 0.
 *
 * ************************************************************************** */

int init_photon_hist(Mu_hist *hist)
{
    double d_theta = 1.0/mu_bins,
           half_width = 0.5 * d_theta;

    hist->bins = malloc(sizeof(double) * mu_bins);
    hist->theta = malloc(sizeof(double) * mu_bins);

    for (int i = 0; i < mu_bins; i++)
    {
        hist->bins[i] = 0;
        hist->theta[i] = acos(i * d_theta + half_width);
    }

    return 0;
}

/* ************************************************************************** */
/** bin_photon
 *
 *  @brief Increment the counter for a photon escape angle.
 *
 *  @param[in, out] Mu_hist *hist. An initialised Mu_hist struct.
 *  @param[in] double cos_theta. A photon's escape angle, mu = cos(theta).
 *
 *  @return 0.
 *
 *  @details
 *
 *  Takes in an initialised Mu_hist struct and a photon's escape angle. The
 *  escape angle is convereted into a binned angle and then the relevant bin
 *  counter is incremented by 1.
 *
 * ************************************************************************** */

int bin_photon(Mu_hist *hist, double cos_theta)
{
    int bin_index = abs((int) (cos_theta * mu_bins));
    hist->bins[bin_index] += 1;

    return 0;
}

/* ************************************************************************** */
/** calculate_intensity
 *
 *  @brief Calculate the normalised(?) intensity for the binned escape angles.
 *
 *  @param[in] Mu_hist *hist. An initialised Mu_hist struct after the monte
 *  carlo iterations have been completed.
 *  @param[in, out] double *intensity. A pointer to an array of mu_bins elements
 *  which will contain the intensity of each binned angle.
 *
 *  @return 0
 *
 *  @details
 *
 *  Takes in a Mu_hist struct after the monte carlo iterations are all complete
 *  and calculates the normalised intensity of each binned photon escape angle.
 *
 * ************************************************************************** */

int calculate_intensity(Mu_hist *hist, double *intensity)
{
    for (int i = 0; i < mu_bins; i++)
    {
        intensity[i] = (hist->bins[i] * mu_bins)/(2 * n_photons *
            cos(hist->theta[i]));
    }

    return 0;
}
