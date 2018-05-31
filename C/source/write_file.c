#include <stdio.h>
#include <stdlib.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Write the binned angles and observables to file.
 *
 */

int write_to_file(Photon_hist *hist, double *intensity)
{
    FILE *write_file;

    if ((write_file = fopen(OUTPUT_FILE, "w")) == NULL)
    {
        printf("Cannot open '%s' for writing.", OUTPUT_FILE);
        exit(-1);
    }

    fprintf(write_file, "%s", "theta\tcounts\tintensity\n");
    for (int i = 0; i < mu_bins; i++)
    {
        fprintf(write_file, "%f\t%d\t%f\n", hist->theta[i],
                hist->bins[i], intensity[i]);
    }

    if (fclose(write_file) != 0)
    {
        printf("%s file could not be closed.", OUTPUT_FILE);
        exit(-1);
    }

    return 0;
}
