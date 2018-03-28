#include <stdio.h>
#include <stdlib.h>

#include "plane.h"

/** @brief Writes the intensity and photon packet count at each binned angle
 *         out to a file named intensity.txt.
 *
 *  Write the output of the simulation to file.
 *
 *  @param *histPtr: A pointer to the struct with the binned angles and counts.
 *         *intensity: A pointer to the array containing the intensities.
 *         mu_bins: int. The number of binned angles.
 *  @return 0, if all goes well.
 */

int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins)
{
    int i;

    FILE *writeFile;

    /* write the output to file */
    if ((writeFile = fopen("intensity.txt", "w")) == NULL)
    {
        printf("Cannot open 'intensity.txt' for writing.");
        exit(-1);
    }

    fprintf(writeFile, "%s", "theta\tcounts\tintensity\n");
    for (i = 0; i < mu_bins; i++)
    {
        fprintf(writeFile, "%f\t%d\t%f\n", histPtr -> theta[i],
                histPtr -> bins[i], intensity[i]);
    }

    if (fclose(writeFile) != 0)
    {
        printf("File could not be closed.");
        exit(-1);
    }

    return 0;
}
