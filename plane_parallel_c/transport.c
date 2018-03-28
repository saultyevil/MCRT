#include <math.h>
#include <stdio.h>

#include "plane.h"

/** @brief Transport a photon packet through an atmosphere with isotropic
 *         scattering.
 *
 *  Transports a photon packet through an atmosphere of constant density by
 *  randomly smapling the optical depth that the packet will move through. The
 *  net distance the packet will move is tau/tau_max, where tau is the sampled
 *  optical depth. After the photon packet is moved, the photon packet is
 *  scattered isotropically and has its direction changed. Diagnostic
 *  information about the photon packet's interactions can be printed to the
 *  screen if required.
 *
 *  @param *packetPtr, albedo, tau_max, printMsg.
 *  @return void.
 */

void transport_photon(struct photon *packetPtr, double albedo, double tau_max)
{
    int int_count;
    double tau, L, L_tot = 0, tau_tot = 0;

    while (packetPtr -> z >= 0.0 && packetPtr -> z <= 1.0)
    {
        /* sample a random optical depth and calculate the dist L */
        tau = -1.0 * log(random_number());
        L = tau/tau_max;

        /* update the position of the packet */
        packetPtr -> x += L * packetPtr -> sintheta * packetPtr -> cosphi;
        packetPtr -> y += L * packetPtr -> sintheta * packetPtr -> sinphi;
        packetPtr -> z += L * packetPtr -> costheta;

        /* if z < 0, has gone into the atmosphere therefore restart the
           packet */
        if (packetPtr -> z < 0.0)
        {
            *packetPtr = *emit_photon();
        }

        /* isotropic scattering */
        if (random_number() < albedo && packetPtr -> z <= 1.0)
        {
            packetPtr -> costheta = 2 * random_number() -1;
            packetPtr -> sintheta = sqrt(1 - (packetPtr -> costheta) *
                                        (packetPtr -> costheta));
            packetPtr -> phi = 2 * M_PI * random_number();
            packetPtr -> cosphi = cos(packetPtr -> phi);
            packetPtr -> sinphi = sin(packetPtr -> phi);
        }

        int_count++;
        L_tot += L;
        tau_tot += tau;
    }

    /* used to keep track of the average interactions happening */
    packetPtr -> avg_L = L_tot/int_count;
    packetPtr -> avg_tau = tau_tot/int_count;
    packetPtr -> interactions = int_count;
}