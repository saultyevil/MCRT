/* ************************************************************************** */
/** @file transport.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for controlling the transport of photons, i.e. the
 *  majority of the good Monte Carlo stuff.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#if defined (_OPENMP)
    #include <omp.h>
#endif

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** isotropic_emit_photon
 *
 *  @brief Place a photon in the origin of the cell pointing in a random
 *  direction.
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC iteration
 *  photon.
 *
 *  @return 0
 *
 *  @details
 *
 *  Places a photon at the origin of the semi-infinte slab pointing in a
 *  randomly chosen direction.
 *
 * ************************************************************************** */

int isotropic_emit_photon(Photon *packet)
{
    double theta, phi;

    random_theta_phi(&theta, &phi);

    packet->x = 0.0;
    packet->y = 0.0;
    packet->z = 0.0;
    packet->cos_phi = cos(phi);
    packet->sin_phi = sin(phi);
    packet->cos_theta = sqrt(gsl_rand_num(0, 1));
    packet->sin_theta = sqrt(1 - packet->cos_theta * packet->cos_theta);

    packet->absorb = FALSE;
    packet->n_inters = 0;

    return 0;
}

/* ************************************************************************** */
/** photon_pos_step
 *
 *  @brief Update the photon's position a distance ds.
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC iteration
 *  photon.
 *  @param[in] double ds. The distance to be travelled by the photon.
 *
 *  @return 0
 *
 *  @details
 *
 *  Updates the position where it can move a radius ds in a random direction
 *  given by the sin and cos of the mu and phi directions of a photon.
 *
 * ************************************************************************** */

int photon_pos_step(Photon *packet, double ds)
{
    packet->x += ds * packet->sin_theta * packet->cos_phi;
    packet->y += ds * packet->sin_theta * packet->sin_phi;
    packet->z += ds * packet->cos_theta;

    return 0;
}

/* ************************************************************************** */
/** transport_photon_const_rho
 *
 *  @brief Function for transport a photon through a cell of constant density.
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC iteration
 *  photon.
 *  @param[in, out] Mu_hist *hist. A pointer to an initialised Mu_hist struct.
 *
 *  @return 0
 *
 *  @details
 *
 *  Transports a photon through a cell in steps of distance ds which is
 *  determined by a randomly sampled optical depth. This continues until either
 *  the photon has escaped the cell, i.e. packet->z > 1, or until the photon
 *  is absorbed. If the photon escapes, its escape angle is binned and the next
 *  photon is transported. If a photon goes back into the atmosphere, i.e.
 *  packet->z < 0, then the photon is remitted.
 *
 *  Photon transport via the Modified Random Walk procedure is also possible
 *  once a photon has scatted a critical amount of times.
 *
 * ************************************************************************** */

int transport_photon_const_rho(Photon *packet, Mu_hist *hist,
    JHK_Moments *moments)
{
    double ds, xi, z_orig;

    isotropic_emit_photon(packet);

    while (packet->z >= 0.0 && packet->z <= 1.0)
    {
        z_orig = packet->z;
        ds = random_tau()/tau_max;
        photon_pos_step(packet, ds);

        calculate_moments(moments, z_orig, packet->z, packet->cos_theta);

        if (packet->z < 0.0)
        {
            isotropic_emit_photon(packet);
        }
        else if (packet->z <= 1.0)
        {
            xi = gsl_rand_num(0, 1);

            if (xi < albedo)  // scatter photon's direction and continue
            {
                random_isotropic_direction(packet);
                packet->n_inters += 1;
            }
            else             // absorb photon and break while loop
            {
                packet->absorb = TRUE;
                packet->n_inters += 1;
                break;
            }
        }
    }

    if (packet->absorb == FALSE)
        bin_photon(hist, packet->cos_theta);

   return 0;
}

/* ************************************************************************** */
/** start_mcrt
 *
 *  @brief The main control function for the MCRT iterations.
 *
 *  @return 0
 *
 *  @details
 *
 *  Controls the flow of the MCRT iterations. The MCRT variables are initialised
 *  at the start of the function. As MCRT is very easy to parallelise, the main
 *  MCRT loop is parallelised using OpenMP. Once the MCRT iterations are
 *  complete, the intensity of the binned escape angles are calculated and then
 *  wirrten to file.
 *
 * ************************************************************************** */

int start_mcrt(void)
{
    #if defined(_OPENMP)
        int omp_counter = 0;
    #endif

    int total_inters = 0;
    double *intens = malloc(sizeof(double) * mu_bins);

    Photon packet;
    Mu_hist hist;
    JHK_Moments moments;

    init_photon_hist(&hist);
    init_jhk(&moments);

    #pragma omp parallel for default(none), \
        schedule(dynamic), \
        shared(n_photons, hist, moments, output_freq, omp_counter), \
        private(packet), \
        reduction(+:total_inters)
    for (int photon_count = 1; photon_count <= n_photons; photon_count++)
    {
        transport_photon_const_rho(&packet, &hist, &moments);
        total_inters += packet.n_inters;

        #if defined(_OPENMP)
            #pragma omp atomic
                omp_counter += 1;

            #pragma omp critical
            {
                if (omp_counter % output_freq == 0)
                    printf("%6.0d photon packets transported (%3.0f%%)\n",
                            omp_counter, (double) omp_counter/n_photons * 100);
            }
        #else
            if (photon_count % output_freq == 0)
                printf("%6.0d photon packets transported (%3.0f%%)\n",
                       photon_count, (double) photon_count/n_photons * 100);
        #endif
    }

    /*
     * Calculate the intensity of the output and dump the results to file
     */
    write_moments_to_file(&moments);
    calculate_intensity(&hist, intens);
    write_intensity_to_file(&hist, intens);

    free(intens);
    free_mu_hist(&hist);
    free_moment_arrays(&moments);

    int average_inters = (int) ((double) total_inters/n_photons);

    printf("\n-------------\n");
    printf("\nTotal number of interactions: %d\n", total_inters);
    printf("Average photon interactions to escape: %d.\n", average_inters);

    return 0;
}
