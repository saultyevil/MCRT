#include <stdlib.h>
#include <math.h>

#include "plane.h"

/** @brief Creates a photon packet which is at the centre of the cell (0, 0, 0),
 *         pointing in a randomly sampled direction.
 *
 *  A photon packet is created at the centre of origin of a cell (0, 0, 0) and
 *  is pointed randomly in a direction. The sines and consines of theta and phi
 *  (the directions) are calculated and used as this generally seems to be more
 *  computationally efficient. The counter variables for tracking the
 *  interactions of the packet are set to 0.
 *
 *  @param Void.
 *  @return *packetPtr: pointer to struct.
 */

struct photon *emit_photon(void)
{
    struct photon *packetPtr = malloc(sizeof(struct photon));

    /* set location to origin of the cell */
    packetPtr -> x = 0.0;
    packetPtr -> y = 0.0;
    packetPtr -> z = 0.0;

    /* sample a theta direction */
    packetPtr -> costheta = sqrt(random_number());
    packetPtr -> sintheta = sqrt(1 - (packetPtr -> costheta) *
                                 (packetPtr -> costheta));

    /* sample a phi direction */
    packetPtr -> phi = 2 * M_PI * random_number();
    packetPtr -> cosphi = cos(packetPtr -> phi);
    packetPtr -> sinphi = sin(packetPtr -> phi);

    /* counters for keeping track of the packet interactions */
    packetPtr -> interactions = 0;
    packetPtr -> avg_tau = 0;
    packetPtr -> avg_L = 0;

    return packetPtr;
}
