#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N_PHOTONS 1000000
#define TAU_MAX 7
#define ALBEDO 1
#define MU_BINS 10
#define PI 3.14159265359

/*-------------------------- struct definitions -----------------------------*/

/* photon packet */
struct photon {
    double x;
    double y;
    double z;
    double costheta;
    double sintheta;
    double cosphi;
    double sinphi;
    double phi;
};

/* histogram for binned angles and count */
struct photon_hist {
    int bins[MU_BINS];
    double theta[MU_BINS];
};

/*---------------------------------------------------------------------------*/


/*-------------- function prototypes of all the functions -------------------*/

/* generates random numbers */
double random_number(void);
/* returns a photon struct which is a photon pointing in a random direction
   at the origin of the slab */
struct photon *emit_photon(void);
/* isotropically scatters a photon packet */
int *scatter_photon(struct photon *packetPtr);
/* bins the escape angles of the photon packets and creates a histogram */
struct photon_hist *bin_photons(double mu[]);
/* calculates the moments of the radiation field */
int *calculate_intensity(struct photon_hist *hist, double *p);
/* writes out the data to file */
int write_to_file(struct photon_hist *histPtr, double *intensity);

/*---------------------------------------------------------------------------*/

/* ---------------------------- main function -------------------------------*/
int main(void){

    /* initalise all of the variables */
    int photon_count, i;
    double MU[N_PHOTONS];
    double PHI[N_PHOTONS];
    double intensity[MU_BINS];
    struct photon packet;
    struct photon_hist hist;
    char* c_time_string;
    time_t current_time;

    /* initalise all of the pointers and point them at something */
    struct photon *packetPtr;
    packetPtr = &packet;
    struct photon_hist *histPtr;
    histPtr = &hist;
    double *intensityPtr;
    intensityPtr = intensity;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    printf("Beginning simulation...\n");
    printf("Current time: %s\n", c_time_string);

    /* set the seed for rand() */
    srand(time(NULL));
    
    for (photon_count = 0; photon_count < N_PHOTONS; photon_count++){
        /* emit, scatter the photon packet and then record its escape angle */
        *packetPtr = *emit_photon();
        *scatter_photon(packetPtr);
        MU[photon_count] = packetPtr -> costheta;

        /* provides a progress report */
        if ((photon_count + 1) % 100000 == 0){
            printf("%6.0d photon packets transported (%3.0f%%)\n",
            (photon_count + 1),(photon_count + 1.0)/N_PHOTONS * 100.0);
        }
    }

    /* bin the photons and calculate the intensity */
    *histPtr = *bin_photons(MU);
    calculate_intensity(histPtr, intensityPtr);
    write_to_file(histPtr, intensity);

    printf("\nSimulation completed.\n");
    return 0;
}

/*---------------------------------------------------------------------------*/

/* return a random number between 0 and 1 */
double random_number(void){

    return (double)rand()/(double)RAND_MAX;
}

/* create a photon packet in a random direction at the origin */
struct photon *emit_photon(void){
    double xi1, xi2;
    double costheta, phi;
 
    struct photon *packetPtr;
    struct photon packet;
    packetPtr = &packet;
 
    /* generate 2 random numbers and the two directions */
    xi1 = random_number();
    xi2 = random_number();
    costheta = sqrt(xi1);
    phi = 2 * M_PI * xi2;
 
    /* populate the photon struct with values */
    packetPtr -> x = 0.0;
    packetPtr -> y = 0.0;
    packetPtr -> z = 0.0;
    packetPtr -> costheta = costheta;
    packetPtr -> sintheta = sqrt(1 - costheta * costheta);
    packetPtr -> cosphi = cos(phi);
    packetPtr -> sinphi = sin(phi);
    packetPtr -> phi = phi;
 
    return packetPtr;
}

/* isotropically scatter a photon */
int *scatter_photon(struct photon *packetPtr){
    double tau, L, xi;
 
    while (packetPtr -> z >= 0.0 && packetPtr -> z <= 1.0){
        /* sample a random optical depth and calculate the dist L */
        tau = -1.0 * log(random_number());
        L = tau/TAU_MAX;
 
        /* update the position of the packet */
        packetPtr -> x += L * packetPtr -> sintheta * packetPtr -> cosphi;
        packetPtr -> y += L * packetPtr -> sintheta * packetPtr -> sinphi;
        packetPtr -> z += L * packetPtr -> costheta;
 
        /* if z < 0, has gone into the atmosphere therefore restart the
           packet */
        if (packetPtr -> z < 0.0){
            *packetPtr = *emit_photon();
        }
 
        xi = random_number();
        /* scatter the photon is random number is less than albedo */
        if (xi < ALBEDO && packetPtr -> z < 1.0){
            packetPtr -> costheta = 2 * random_number() -1;
            packetPtr -> sintheta = sqrt(1 - (packetPtr -> costheta) *
                                        (packetPtr -> costheta));
            packetPtr -> phi = 2 * M_PI * random_number();
            packetPtr -> cosphi = cos(packetPtr -> phi);
            packetPtr -> sinphi = sin(packetPtr -> phi);
        }
    }
 
    return 0;
}

/* bin the packet escape angles */
struct photon_hist *bin_photons(double mu[]){
    int i, bin_index;
    double dtheta, half_width;
 
    struct photon_hist *histPtr;
    struct photon_hist hist;
    histPtr = &hist;
 
    dtheta = 1.0/MU_BINS;
    half_width = 0.5 * dtheta;
 
    /* calculate the binned angles */
    for (i = 0; i < MU_BINS; i++){
        histPtr -> bins[i] = 0; // set the bin count to zero
        histPtr -> theta[i] = acos(i * dtheta + half_width) * (180/M_PI);
    }
 
    /* bin the escape angles accordinly */
    for (i = 0; i < N_PHOTONS; i++){
        bin_index = abs((int)(mu[i] * MU_BINS));
        histPtr -> bins[bin_index] += 1;
    }
 
    return histPtr;
}

/* calculate the intensity */
int *calculate_intensity(struct photon_hist *hist, double *p){
    int i;
 
    for (i = 0; i < MU_BINS; i++){
        p[i] = ((float)hist -> bins[i] * MU_BINS)/(2 * N_PHOTONS * cos(
            hist -> theta[i] * M_PI/180));
    }
 
    return 0;
}

/* output the data to file */
int write_to_file(struct photon_hist *histPtr, double *intensity){
    int i;

    FILE *writeFile;

    /* write the output to file */
    if ((writeFile = fopen("intensity.dat", "w")) == NULL){
        printf("Cannot open 'intensity.dat' for writing.");
        return -1;
    }

    fprintf(writeFile, "%s", "theta\tcounts\tintensity\n");
    for (i = 0; i < MU_BINS; i++){
        fprintf(writeFile, "%f\t%d\t%f\n", histPtr -> theta[i],
                histPtr -> bins[i], intensity[i]);
    }

    if (fclose(writeFile) != 0){
        printf("File could not be closed.");
        return -1;
    }

    return 0;
}
