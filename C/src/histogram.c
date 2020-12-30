/* ************************************************************************** */
/** @file intensity.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for creating and initialising the photon escape
 *  angle weight, as well as functions used to bin photons into the correct bin
 *  and calculating the intensity of the binned angles.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** init_histogram
 *
 *  @brief Initialise a Histogram_t structure with the escape angles.
 *
 *  @param[in, out] *hist  Pointer to an uninitialised Histogram_t struct.
 *
 *  @details
 *
 *  Takes in an uninitialised Histogram_t struct, allocates memory for the
 *  pointers within the struct with mu_bin elements and calculates the
 *  bin's escape angle value and sets the bin counts to 0.
 *
 * ************************************************************************** */

void
init_histogram(Histogram_t *hist)
{
  double d_theta = 1.0 / hist->n_bins;
  double half_width = 0.5 * d_theta;

  hist->weight = calloc(hist->n_bins, sizeof *hist->weight);
  hist->theta = calloc(hist->n_bins, sizeof *hist->theta);
  hist->intensity = calloc(hist->n_bins, sizeof *hist->intensity);

  for(int i = 0; i < hist->n_bins; i++)
    hist->theta[i] = acos(i * d_theta + half_width);
}

/* ************************************************************************** */
/** bin_photon_to_histogram
 *
 *  @brief Increment the escape angle bin counter.
 *
 *  @param[in, out] Mu_hist *hist. An initialised Histogram_t struct.
 *
 *  @param[in] double costheta. A photon's escape angle, mu = cos(theta).
 *
 *  @return 0.
 *
 *  @details
 *
 *  Converts the photon's escape angle into a binned angle index and increments
 *  the bin count by 1 for that escape angle.
 *
 * ************************************************************************** */

void
bin_photon_to_histogram(Histogram_t *hist, double costheta)
{
  int index = abs((int) (costheta * hist->n_bins));
  hist->weight[index] += 1;
}

/* ************************************************************************** */
/** convert_weight_to_intensity
 *
 *  @brief Calculate the flux normalised intensity of each binned angle.
 *
 *  @param[in] Mu_hist *hist. An initialised Histogram_t struct after the MC
 *  iterations have been completed.
 *
 *  @param[in, out] double *intensity. A pointer to an empty array with mu_bins
 *  elements.
 *
 *  @return 0
 *
 *  @details
 *
 *  The idea behind calculating the intensity at the various binned angles is
 *  to count the escaped photons at that bin. Thus by doing this, and diving by
 *  the number of photons, the flux normalised intensity of the escape angles
 *  is easily calculated.
 *
 * ************************************************************************** */

void
convert_weight_to_intensity(Histogram_t *hist)
{
  for(int i = 0; i < hist->n_bins; i++)
    hist->intensity[i] = (hist->weight[i] * hist->n_bins) / (2.0 * N_PHOTONS * cos(hist->theta[i]));
}
