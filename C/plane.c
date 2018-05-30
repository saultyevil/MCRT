#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "plane.h"

int main(int argc, char *argv[])
{
    int mu_bins, verbose_output, avg_scatter, avg_scatter_total = 0;
    long n_photons;
    double tau_max, albedo;
    char *ini_file;

    FILE *verbose_file;  // have to declare here for scope reasons
    verbose_output = 0;  // this needs to be sorted out -- have in the ini file?

    printf("\nBeginning simulation...\n");
    print_time();

    if (argc >= 2)
    {
        ini_file = argv[1];

    }
    else
    {
        printf("No configuration file provided in the program arguments.\n");
        printf("Using default filename: %s.\n\n", INI_FILE);
        ini_file = INI_FILE;
    }

    /*
     * Read in variables from file
     */
    read_long("N_PHOTONS", &n_photons, ini_file);
    read_double("TAU_MAX", &tau_max, ini_file);
    read_double("ALBEDO", &albedo, ini_file);
    mu_bins = MU_BINS;  // MU_BINS is defined in the plane.h for now

    /*
     * Initalise arrays and pointers for photon packets and histogram
     */
    double *MU = malloc(sizeof(*MU) * n_photons);
    double *intensity = malloc(sizeof(*intensity) * mu_bins);
    struct photon_hist *hist = malloc(sizeof(struct photon_hist));
    struct photon *packetPtr = malloc(sizeof(struct photon));

    printf("Parameters:\n");
    printf("-----------\n\n");
    printf("N_PHOTONS      : %ld\n", n_photons);
    printf("TAU_MAX        : %3.1f\n", tau_max);
    printf("ALBEDO         : %1.1f\n", albedo);
    printf("MU_BINS        : %d\n", mu_bins);
    printf("VERBOSE_OUTPUT : %d\n\n", verbose_output);
    printf("-----------\n\n");

    srand(time(NULL));

    /*
     * If verbose output is turned on, open the file in write mode
     */
    if (verbose_output == TRUE)
    {
        if ((verbose_file = fopen(VERBOSE_OUTPUT_FILE, "w")) == NULL)
        {
            printf("Cannot open verbose output file '%s'.\n",
                    VERBOSE_OUTPUT_FILE);
            exit(-1);
        }

        fprintf(verbose_file, "Packet\tScatters\tTau\tDisp\n");
    }

    /*
     * MC interations
     */
    for (int photon_count = 1; photon_count <= n_photons; photon_count++)
    {
        /*
         * emit a packet, scatter it and record the escape angle
         */
        emit_photon(packetPtr);
        transport_photon(packetPtr, albedo, tau_max);
        MU[photon_count-1] = packetPtr -> costheta;

        /*
         * Output the data to file for each photon packet if verbose logging is
         * enabled by the user
         */
        if (verbose_output == TRUE)
        {
            fprintf(verbose_file, "%d\t%d\t%f\t%f\n", photon_count,
                    packetPtr -> interactions, packetPtr -> avg_tau,
                    packetPtr -> avg_L);
        }

        if (photon_count % 100000 == 0)
        {
            printf("%6.0d photon packets transported (%3.0f%%)\n",
            photon_count, (double) photon_count/n_photons * 100);
        }

        avg_scatter_total += packetPtr -> interactions;
    }

    if (verbose_output == TRUE)
    {
        if (fclose(verbose_file) != 0)
        {
            printf("'%s' could not be closed.", VERBOSE_OUTPUT_FILE);
            exit(-1);
        }
    }

    /*
     * Bin the photons and calculate the intensity and write to file
     */
    bin_photons(hist, MU, n_photons, mu_bins);
    calculate_intensity(intensity, hist, n_photons, mu_bins);
    write_to_file(hist, intensity, mu_bins);

    free(packetPtr); free(MU); free(hist); free(intensity);

    avg_scatter = round((double) avg_scatter_total/n_photons);

    printf("\nAverage number of scatter interactions per photon: %d.\n",
           avg_scatter);
    printf("\nSimulation completed.\n");

    return 0;
}
