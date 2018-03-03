#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "plane.h"

int main(void){

    /* to do: par file to read in the parameters */
    int photon_count, mu_bins;
    long n_photons;
    double tau_max, albedo;

    n_photons = 1000000;
    tau_max = 10;
    albedo = 1;

    /* have parameters now, so create the arrays & variables */
    mu_bins = MU_BINS;
    struct photon packet;
    struct photon_hist hist;
    double MU[n_photons], intensity[mu_bins];

    /* initalise all of the pointers and point them at something */
    double *intensityPtr;
    struct photon *packetPtr;
    struct photon_hist *histPtr;
    packetPtr = &packet;
    histPtr = &hist;
    intensityPtr = intensity;
\
/* --------------------------- Start the simulation ------------------------ */

    printf("Beginning simulation...\n");
    print_time();

    /* set the seed for rand() */
    srand(time(NULL));

    for (photon_count = 0; photon_count < n_photons; photon_count++){
        /* emit, scatter the photon packet and then record its escape angle */
        *packetPtr = *emit_photon();
        *scatter_photon(packetPtr, albedo, tau_max);
        MU[photon_count] = packetPtr -> costheta;

        /* provides a progress report */
        if ((photon_count + 1) % 100000 == 0){
            printf("%6.0d photon packets transported (%3.0f%%)\n",
            (photon_count + 1), (photon_count + 1.0)/n_photons * 100.0);
        }
    }

    /* bin the photons and calculate the intensity */
    *histPtr = *bin_photons(MU, n_photons, mu_bins);
    calculate_intensity(histPtr, intensityPtr, n_photons, mu_bins);
    write_to_file(histPtr, intensity, mu_bins);

    printf("\nSimulation completed.\n");
    return 0;
}
