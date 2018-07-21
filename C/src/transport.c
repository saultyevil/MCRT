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

    random_cost_phi(&theta, &phi);

    packet->absorb = FALSE;
    packet->n_inters = 0;

    packet->x = 0.0;
    packet->y = 0.0;
    packet->z = 0.0;
    packet->cos_phi = cos(phi);
    packet->sin_phi = sin(phi);
    packet->cos_theta = cos(theta);
    packet->sin_theta = sin(theta);

    return 0;
}

/* ************************************************************************** */
/** photon_pos_step
 *
 *  @brief Transport the photon a distance ds, in a direction (theta, phi).
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC photon packet.
 *
 *  @param[in] double ds. The displacement for the photon to travel.
 *
 *  @return 0
 *
 *  @details
 *
 *  Updates the photon packet position in x, y and z depending on the distance
 *  ds and the direction (theta, phi). The position is updated using equations
 *  for a "spherical step".
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
 *  @brief Control photon transport within in a slab of constant density.
 *
 *  @param[in, out] Photon *packet. A pointer to the current MC photon packet.
 *
 *  @param[in, out] Mu_hist *hist. A pointer to an initialised Mu_hist struct.
 *
 *  @param[in, out] JHK_Moments *moments. A pointer to an initialised
 *  JHK_Moments struct.
 *
 *  @return 0
 *
 *  @details
 *
 *  Transports a photon through a cell in steps of ds. The distance ds is
 *  determined by sampling from a negative exponential function for an optical
 *  depth and converting this optical depth into a physical distance. The
 *  position of the photon packet is tracked in Cartersian coordinates, but
 *  can also be tracked in terms of optical depth position in the slab.
 *
 *  Photons are transported until they either escape the slab, i.e.
 *  packet->z > 1, or until the photon is absorbed. If the photon escapes, its
 *  escape angle is binned and the next photon is transported.
 *
 *  However, if a photon goes below the slab, i.e. packet->z < 0, then the
 *  photon is remitted and transported again to enforce the boundary flux
 *  condition of n_photons.
 *
 *  In optical depth coordinates, a photon will continue to be transported if
 *  packet->tau_coord >= 0 && packet->tau_coord <= tau_max.
 *
 *  If a photon becomes trapped in an optically thick, the Modified Random
 *  Walk will be invoked to macrostep the photon. This will only occur when the
 *  photon has undergone mrw_critical_scatters scattering events. After each
 *  MRW step, this counter is reset to limit the number of MRW transport steps
 *  which can be done to help limit the number of MRW transport steps taken. If
 *  too many MRW steps are taken, this can limit the accuracy of the simulation.
 *
 *  After each position update, the J, H and K moments of the radiation field
 *  within the slab are updated accordinly to the number of levels the photon
 *  traversed between updates.
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
        packet->n_inters += 1;

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
            }
            else             // absorb photon and break while loop
            {
                packet->absorb = TRUE;
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
 *  @brief Control the MC interations.
 *
 *  @return 0
 *
 *  @details
 *
 *  Controls the flow of the MCRT iterations. The MCRT variables are initialised
 *  at the start of the function. As MCRT is very easy to parallelise, the main
 *  MCRT loop is parallelised using OpenMP.
 *
 *  Once the MCRT iterations are complete, the intensity of the binned escape
 *  angles is calculated and then written to file, as well as the moments
 *  of the radiation of the field within the slab.
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
        schedule(static), \
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
    calculate_intensity(&hist, intens);
    write_intensity_to_file(&hist, intens);
    write_moments_to_file(&moments);

    /*
     * Cleaning up...
     */
    free(intens);
    free_mu_hist(&hist);
    free_moment_arrays(&moments);

    int average_inters = (int) ((double) total_inters/n_photons);

    printf("\n-------------\n");
    printf("\nTotal number of interactions: %d\n", total_inters);
    printf("Average photon interactions to escape: %d.\n", average_inters);

    return 0;
}
