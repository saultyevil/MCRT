#include <stdio.h>
#include <stdlib.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Write the binned angles and observables to file.
 *
 */

int write_to_file(Photon_hist *hist, Moments *moments, double *intensity)
{
    FILE *write_file_intens = NULL;
    FILE *write_file_moments = NULL;

    /*
     * Write out the intensity data to file
     */
    if ((write_file_intens = fopen(OUTPUT_FILE_INTENS, "w")) == NULL)
    {
        printf("Cannot open file '%s' for writing.\n", OUTPUT_FILE_INTENS);
        exit(-1);
    }

    print_par_header(write_file_intens, 'I');

    for (int i = 0; i < mu_bins; i++)
        fprintf(write_file_intens, "%f\t%d\t%f\n", hist->theta[i],
                hist->bins[i], intensity[i]);

    if (fclose(write_file_intens) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_INTENS);
        exit(-1);
    }

    /*
     * Write out the moments data to file
     */
    if ((write_file_moments = fopen(OUTPUT_FILE_MOMENTS, "w")) == NULL)
    {
        printf("Cannot open file '%s' for writing.\n", OUTPUT_FILE_MOMENTS);
        exit(-1);
    }

    print_par_header(write_file_moments, 'M');

    for (int i = 0; i < n_levels; i++)
        fprintf(write_file_moments, "%d\t%f\t%f\t%f\n",
                i+1,
                moments->j_plus[i] + moments->j_minus[i],
                moments->h_plus[i] + moments->h_minus[i],
                moments->k_plus[i] + moments->k_minus[i]);

    if (fclose(write_file_moments) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_MOMENTS);
        exit(-1);
    }

    return 0;
}

int print_par_header(FILE *write_file, int out_file)
{
    fprintf(write_file, "Parameters:\n");
    fprintf(write_file, "-----------\n");
    fprintf(write_file, "N_PHOTONS      : %d\n", n_photons);
    fprintf(write_file, "MU_BINS        : %d\n", mu_bins);
    fprintf(write_file, "N_LEVELS       : %d\n", n_levels);
    fprintf(write_file, "OUTPUT_FREQ    : %d\n", output_freq);
    fprintf(write_file, "TAU_MAX        : %4.3f\n", tau_max);
    fprintf(write_file, "ALBEDO         : %4.3f\n", albedo);
    fprintf(write_file, "-----------\n\n");

    /*
     * These are the appropriate headers for each file.
     *  - 'I' for intensity.txt
     *  - 'M' for moments.txt
     */
    switch (out_file)
    {
        case 'I':
            fprintf(write_file, "%s", "theta\tcounts\tintensity\n");
            break;
        case 'M':
            fprintf(write_file, "%s", "level\tj\th\tk\n");
            break;
    }

    return 0;
}
