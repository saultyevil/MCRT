#include <stdio.h>

#include "plane.h"

/* output the data to file */
int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins){
    int i;

    FILE *writeFile;

    /* write the output to file */
    if ((writeFile = fopen("intensity.dat", "w")) == NULL){
        printf("Cannot open 'intensity.dat' for writing.");
        return -1;
    }

    fprintf(writeFile, "%s", "theta\tcounts\tintensity\n");
    for (i = 0; i < mu_bins; i++){
        fprintf(writeFile, "%f\t%d\t%f\n", histPtr -> theta[i],
                histPtr -> bins[i], intensity[i]);
    }

    if (fclose(writeFile) != 0){
        printf("File could not be closed.");
        return -1;
    }

    return 0;
}
