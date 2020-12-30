/* ************************************************************************** */
/** @file write_file.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Various functions for writing simulation results out to file.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** ouput_intensity_to_file
 *
 *  @brief Write the intensity of the binned escaped angles to file.
 *
 *  @param[in] intensity_histogram *hist   A Histogram_t struct after the MCRT
 *                                         iterations.
 *
 *  @details
 *
 * ************************************************************************** */

void
ouput_intensity_to_file(Histogram_t *hist)
{
  int i;
  FILE *f = NULL;

  if((f = fopen(OUTPUT_FILE_INTENS, "w")) == NULL)
  {
    printf("Cannot open file %s\n", OUTPUT_FILE_INTENS);
    return;
  }

  fprintf(f, "%-12s %-12s %-12s\n", "angle", "weight", "intensity");

  for(i = 0; i < hist->n_bins; i++)
    fprintf(f, "%-12f %-12e %-12e\n", hist->theta[i], hist->weight[i], hist->intensity[i]);

  if(fclose(f))
  {
    printf("Cannot close file %s\n", OUTPUT_FILE_INTENS);
    exit(-1);
  }
}

/* ************************************************************************** */
/** output_radiation_moments_to_file
 *
 *  @brief Write the JHK moments of the radiation field to file.
 *
 *  @param[in] Moments *moments. An initialised Moments_t struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

void
output_radiation_moments_to_file(Moments_t *moments)
{
  int i;
  FILE *f = NULL;

  if((f = fopen(OUTPUT_FILE_MOMENTS, "w")) == NULL)
  {
    printf("Cannot access file %s\n", OUTPUT_FILE_MOMENTS);
    exit(-1);
  }

  fprintf(f, "%-12s %-12s %-12s %-12s %-12s %-12s %-12s\n", "level", "j_plus", "j_minus", "h_plus", "h_minus",
    "k_plus", "k_minus");

  for(i = 0; i < moments->n_levels + 1; i++)
  {
    fprintf(f, "%-12d %-12e %-12e %-12e %-12e %-12e %-12e\n", i + 1,
      moments->j_plus[i] / N_PHOTONS, moments->j_minus[i] / N_PHOTONS,
      moments->h_plus[i] / N_PHOTONS, moments->h_minus[i] / N_PHOTONS,
      moments->k_plus[i] / N_PHOTONS, moments->k_minus[i] / N_PHOTONS);
  }

  if(fclose(f))
  {
    printf("Cannot close file %s\n", OUTPUT_FILE_MOMENTS);
    exit(-1);
  }
}
