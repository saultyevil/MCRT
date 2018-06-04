#include <stdio.h>
#include <stdlib.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Write the binned angles and observables to file.
 *
 */

int write_to_file(Photon_hist *hist, Moments *moments, double *intensity)
{
    FILE *write_file_intens;
    FILE *write_file_moments;

    /*
     * Write out the intensity data to file
     */
    if ((write_file_intens = fopen(OUTPUT_FILE_INTENS, "w")) == NULL)
    {
        printf("Cannot open file '%s' for writing.\n", OUTPUT_FILE_INTENS);
        exit(-1);
    }

    fprintf(write_file_intens, "%s", "theta\tcounts\tintensity\n");
    for (int i = 0; i < mu_bins; i++)
    {
        fprintf(write_file_intens, "%f\t%d\t%f\n", hist->theta[i],
                hist->bins[i], intensity[i]);
    }

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

    fprintf(write_file_moments, "%s", "level\tj\th\tk\n");
    for (int i = 0; i < n_levels; i++)
    {
        fprintf(write_file_moments, "%d\t%f\t%f\t%f\n",
                i+1,
                moments->j_plus[i] + moments->j_minus[i],
                moments->h_plus[i] + moments->h_minus[i],
                moments->k_plus[i] + moments->k_minus[i]);
    }

    if (fclose(write_file_moments) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_MOMENTS);
        exit(-1);
    }

    return 0;
}
