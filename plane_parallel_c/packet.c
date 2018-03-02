#include <math.h>

#include "plane.h"

/* create a photon packet at the origin */
struct photon *emit_photon(void){
    double costheta, phi;
 
    struct photon *packetPtr;
    struct photon packet;
    packetPtr = &packet;
 
    /* populate the photon struct with values */
    packetPtr -> x = 0.0;
    packetPtr -> y = 0.0;
    packetPtr -> z = 0.0;
    packetPtr -> costheta = sqrt(random_number());
    packetPtr -> sintheta = sqrt(1 - (packetPtr -> costheta) * 
                                 (packetPtr -> costheta));
    packetPtr -> phi = phi = 2 * M_PI * random_number();
    packetPtr -> cosphi = cos(packetPtr -> phi);
    packetPtr -> sinphi = sin(packetPtr -> phi);
 
    return packetPtr;
}

/* isotropically scatter a photon */
int *scatter_photon(struct photon *packetPtr, double albedo, double tau_max){
    double tau, L;
 
    while (packetPtr -> z >= 0.0 && packetPtr -> z <= 1.0){
        /* sample a random optical depth and calculate the dist L */
        tau = -1.0 * log(random_number());
        L = tau/tau_max;
 
        /* update the position of the packet */
        packetPtr -> x += L * packetPtr -> sintheta * packetPtr -> cosphi;
        packetPtr -> y += L * packetPtr -> sintheta * packetPtr -> sinphi;
        packetPtr -> z += L * packetPtr -> costheta;
 
        /* if z < 0, has gone into the atmosphere therefore restart the
           packet */
        if (packetPtr -> z < 0.0){
            *packetPtr = *emit_photon();
        }
 
        /* scatter the photon is random number is less than albedo */
        if (random_number() < albedo && packetPtr -> z <= 1.0){
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