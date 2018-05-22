#include <stdio.h>
#include <stdlib.h>

#include "plane.h"

/** @brief Writes the intensity and photon packet count at each binned angle
 *         out to a file named intensity.txt.
 *
 *  Write the output of the simulation to file.
 */

int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins)
{
    int i;

    FILE *write_file;

    if ((write_file = fopen(OUTPUT_FILE, "w")) == NULL)
    {
        printf("Cannot open '%s' for writing.", OUTPUT_FILE);
        exit(-1);
    }

    /*
     * Write the intensity to file from the histogram
     */
    fprintf(write_file, "%s", "theta\tcounts\tintensity\n");
    for (i = 0; i < mu_bins; i++)
    {
        fprintf(write_file, "%f\t%d\t%f\n", histPtr -> theta[i],
                histPtr -> bins[i], intensity[i]);
    }

    if (fclose(write_file) != 0)
    {
        printf("%s file could not be closed.", OUTPUT_FILE);
        exit(-1);
    }

    return 0;
}
