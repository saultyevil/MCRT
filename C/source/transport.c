#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Controls the MCRT simulation.
 *
 */
int start_mcrt(void)
{
   Photon *packet = malloc(sizeof(Photon));
   Photon_hist *hist = malloc(sizeof(Photon_hist));

    init_photon_hist(hist);

    for (int photon_count = 1; photon_count <= n_photons; photon_count++)
    {
        emit_photon(packet);
        transport_photon_const_rho(packet);

        if (packet->absorb == FALSE)
            bin_photon(hist, packet->cos_theta);

        if (photon_count % 100000 == 0)
            printf("%6.0d photon packets transported (%3.0f%%)\n",
            photon_count, (double) photon_count/n_photons * 100);
    }

    double *intens = malloc(sizeof(*intens) * mu_bins);

    calculate_intensity(hist, intens);
    write_to_file(hist, intens);

    free(intens);
    free(packet);
    free(hist);

    return 0;
}

/** @brief Transport a photon packet through an atmosphere of constant density.
 *
 */
int transport_photon_const_rho(Photon *packet)
{
    packet->absorb = FALSE;

    while (packet->z >= 0.0 && packet->z <= 1.0)
    {
        double tau = -1.0 * log(gsl_rand_num(0, 1));
        double L = tau/tau_max;

        packet->x += L * packet->sin_theta * packet->cos_phi;
        packet->y += L * packet->sin_theta * packet->sin_phi;
        packet->z += L * packet->cos_theta;

        /*
         * If z < 0, the photon has gone back into the atmosphere, therefore we
         * will restart this packet
         */
        if (packet->z >= 0.0 && packet->z <= 1.0)
        {
            double xi = gsl_rand_num(0, 1);

            if (xi < albedo && packet->z <= 1.0)
            {
                isotropic_scatter(packet);
            }
            else if (xi > albedo)
            {
                packet->absorb = TRUE;
                break;
            }
        }
        else if (packet->z < 0.0)
        {
            emit_photon(packet);
        }
    }

   return 0;
}

/** @brief Isotropic scattering of a photon packet.
 *
 */
int isotropic_scatter(Photon *packet)
{
    packet->cos_theta = 2 * gsl_rand_num(0, 1) -1;
    packet->sin_theta = sqrt(1 - (packet->cos_theta) * (packet->cos_theta));
    packet->cos_phi = cos(2 * PI * gsl_rand_num(0, 1));
    packet->sin_phi = sqrt(1 - (packet->cos_phi) * (packet->cos_phi));;

    return 0;
}

/** @brief Create a photon packet at the origin with a random direction.
 *
 */
int emit_photon(Photon *packet)
{
    packet->x = 0.0;
    packet->y = 0.0;
    packet->z = 0.0;

    packet->cos_theta = sqrt(gsl_rand_num(0, 1));
    packet->sin_theta = sqrt(1 - (packet->cos_theta * packet->cos_theta));
    packet->cos_phi = cos(2 * PI * gsl_rand_num(0, 1));
    packet->sin_phi = sqrt(1 - (packet->cos_phi * packet->cos_phi));

    return 0;
}
