#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "plane.h"

int main(int argc, char *argv[])
{
/* --------------- Initialise and read variables in from file --------------- */

    int mu_bins, term_output;
    long n_photons, avg_scatter_total = 0;
    double tau_max, albedo, avg_scatter;

    n_photons = (long) read_in_par("N_PHOTONS");
    tau_max = read_in_par("TAU_MAX");
    albedo = read_in_par("ALBEDO");
    mu_bins = MU_BINS;  /* MU_BINS is defined in the plane.h */

/* -------------- Initalise structures, arrays and pointers ----------------- */

    /* initalise arrays and pointers for photon packets and histogram */
    double *MU = malloc(sizeof(double) * n_photons);
    double *intensity;
    struct photon_hist *hist;
    struct photon *packetPtr;

/* -------------------- Take in command line arguments ---------------------- */

    /* printMsg is turned off by default */
    term_output = 0;

    if (argc >= 2)
    {
        term_output = atoi(argv[1]);
    }

/* ------------------------ Start the simulation ---------------------------- */

    printf("\nBeginning simulation...\n");
    print_time();
    printf("Parameters:\n");
    printf("-----------\n\n");
    printf("N_PHOTONS   : %ld\n", n_photons);
    printf("TAU_MAX     : %3.1f\n", tau_max);
    printf("ALBEDO      : %1.1f\n", albedo);
    printf("MU_BINS     : %d\n", mu_bins);
    printf("TERM_OUTPUT : %d\n\n", term_output);

    /* set the seed for rand() */
    srand(time(NULL));

/* --------------------------- Photon transport ----------------------------- */

    for (int photon_count = 1; photon_count <= n_photons; photon_count++)
    {
        /* emit a packet, scatter it, record the escape angle */
        packetPtr = emit_photon();
        transport_photon(packetPtr, albedo, tau_max);
        MU[photon_count - 1] = packetPtr -> costheta;

        /* used for diagnostic reasons, prints which photon the loop is on */
        if (term_output == TRUE)
        {
            printf("Photon Packet: %d\n", photon_count);
            printf("Scatter count: %d\n", packetPtr -> interactions);
            printf("Average tau: %f\n", packetPtr -> avg_tau);
            printf("Average displacement: %f\n\n", packetPtr -> avg_L);
        }

        if (photon_count % 100000 == 0)
        {
            printf("%6.0d photon packets transported (%3.0f%%)\n",
            photon_count, (double) photon_count/n_photons * 100);
        }

        avg_scatter_total += packetPtr -> interactions;
    }

    avg_scatter = (double) avg_scatter_total/n_photons;
    free(packetPtr);

/* ------------------------- Create histogram ------------------------------- */

    /* create histogram of binned angles and counts */
    hist = bin_photons(MU, n_photons, mu_bins);
    free(MU);

    /* calculate the intensity and write to file */
    intensity = calculate_intensity(hist, n_photons, mu_bins);
    write_to_file(hist, intensity, mu_bins);

    free(hist);
    free(intensity);

    printf("\nSimulation completed.\n");
    printf("Average number of scatter interactions: %4.1f.\n", avg_scatter);

    return 0;
}
