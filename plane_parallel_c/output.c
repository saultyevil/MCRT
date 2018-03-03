#include <stdlib.h>
#include <math.h>

#include "plane.h"

/* bin the packet escape angles */
struct photon_hist *bin_photons(double mu[], int n_photons, int mu_bins){
    int i, bin_index;
    double dtheta, half_width;

    struct photon_hist *histPtr;
    struct photon_hist hist;
    histPtr = &hist;

    dtheta = 1.0/mu_bins;
    half_width = 0.5 * dtheta;

    /* calculate the binned angles */
    for (i = 0; i < mu_bins; i++){
        histPtr -> bins[i] = 0; // set the bin count to zero
        histPtr -> theta[i] = acos(i * dtheta + half_width) * (180/M_PI);
    }

    /* bin the escape angles accordinly */
    for (i = 0; i < n_photons; i++){
        bin_index = abs((int)(mu[i] * mu_bins));
        histPtr -> bins[bin_index] += 1;
    }

    return histPtr;
}

/* calculate the intensity */
int *calculate_intensity(struct photon_hist *hist, double *intensity,
int n_photons, int mu_bins){
    int i;

    for (i = 0; i < mu_bins; i++){
        intensity[i] = ((float)hist -> bins[i] * mu_bins)/(2 * n_photons *
            cos(hist -> theta[i] * M_PI/180));
    }

    return 0;
}