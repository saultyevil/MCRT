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

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** write_intensity_to_file
 *
 *  @brief Write the intensity of the binned escaped angles to file.
 *
 *  @param[in] Mu_hist *hist. An Mu_hist struct after the MCRT iterations.
 *  @param[in] double *intensity. The intensity for eached binned escape angle.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

int write_intensity_to_file(Mu_hist *hist, double *intensity)
{
    FILE *write_file_intens = NULL;

    if ((write_file_intens = fopen(OUTPUT_FILE_INTENS, "w")) == NULL)
    {
        printf("Cannot access file '%s'.\n", OUTPUT_FILE_INTENS);
        exit(-1);
    }

    fprintf(write_file_intens, "theta\tcounts\tintensity\n");

    for (int i = 0; i < mu_bins; i++)
        fprintf(write_file_intens, "%f\t%d\t%f\n", hist->theta[i],
                hist->bins[i], intensity[i]);

    if (fclose(write_file_intens) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_INTENS);
        exit(-1);
    }

    return 0;
}

/* ************************************************************************** */
/** write_moments_to_file
 *
 *  @brief Write the JHK moments of the radiation field to file.
 *
 *  @param[in] JHK_Moments *moments. An initialised JHK_Moments struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

int write_moments_to_file(JHK_Moments *moments)
{
    FILE *write_file_moments = NULL;

    if ((write_file_moments = fopen(OUTPUT_FILE_MOMENTS, "w")) == NULL)
    {
        printf("Cannot access file '%s'.\n", OUTPUT_FILE_MOMENTS);
        exit(-1);
    }

    fprintf(write_file_moments, "level\tj_plus\tj_minus\th_plus\th_minus");
    fprintf(write_file_moments, "\tk_plus\tk_minus\n");

    for (int i = 0; i < n_levels + 1; i++)
        fprintf(write_file_moments, "%d\t%f\t%f\t%f\t%f\t%f\t%f\n",
               i+1,
               moments->j_plus[i]/n_photons, moments->j_minus[i]/n_photons,
               moments->h_plus[i]/n_photons, moments->h_minus[i]/n_photons,
               moments->k_plus[i]/n_photons, moments->k_minus[i]/n_photons);

    if (fclose(write_file_moments) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_MOMENTS);
        exit(-1);
    }

    return 0;
}
