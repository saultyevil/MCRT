/* ************************************************************************** */
/** @file transport.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains functions for controlling the transport of photons, i.e.
 *  the majority of the Monte Carlo functions.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "variables.h"
#include "functions.h"

#if defined (_OPENMP)
#include <omp.h>
#endif

/* ************************************************************************** */
/** isotropic_emit_photon
 *
 *  @brief Emit a photon at the origin of the plane isotropically.
 *
 *  @param[in,out] *packet  A pointer to the current MC photon packet.
 *
 *  @details
 *
 *  Places a photon at the origin of the semi-infinte slab pointing in a
 *  randomly chosen direction. Various counters and indicator flags are set
 *  to show that the photon is newly emitted.
 *
 * ************************************************************************** */

void
isotropic_emit_photon(PhotonPacket_t *packet)
{
  double mu, phi;
  random_theta_phi(&mu, &phi);
  packet->x = 0.0;
  packet->y = 0.0;
  packet->z = 0.0;
  packet->cosphi = cos(phi);
  packet->sinphi = sin(phi);
  packet->costheta = sqrt(gsl_rand_num(0, 1));
  packet->sintheta = sqrt(1 - packet->costheta * packet->costheta);
  packet->absorb = false;
}

/* ************************************************************************** */
/** move_photon
 *
 *  @brief Transport the photon a distance ds, in a direction (theta, phi).
 *
 *  @param[in,out] *packet  A pointer to the current MC photon packet.
 *  @param[in] ds           The displacement for the photon to travel.
 *
 *  @details
 *
 *  Updates the photon packet position in x, y and z depending on the distance
 *  ds and the direction (theta, phi). The position is updated using equations
 *  for a "spherical step".
 *
 * ************************************************************************** */

void
move_photon(PhotonPacket_t *packet, double ds)
{
  packet->x += ds * packet->sintheta * packet->cosphi;
  packet->y += ds * packet->sintheta * packet->sinphi;
  packet->z += ds * packet->costheta;
}

/* ************************************************************************** */
/** transport_single_photon
 *
 *  @brief Control photon transport within in a slab of constant density.
 *
 *  @param[in, out] *hist     A pointer to an initialised Histogram_t struct.
 *  @param[in, out] *moments  A pointer to an initialised Moments_t struct.
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
 *  condition of N_PHOTONS.
 *
 *  In optical depth coordinates, a photon will continue to be transported if
 *  packet->tau_coord >= 0 && packet->tau_coord <= TAU_MAX.
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

void
transport_single_photon(Histogram_t *hist, Moments_t *moments)
{
  PhotonPacket_t photon = PHOTON_INIT;

  isotropic_emit_photon(&photon);

  double z_orig;
  while(photon.z >= 0.0 && photon.z <= 1.0)
  {
    z_orig = photon.z;
    double ds = random_tau() / TAU_MAX;
    move_photon(&photon, ds);
    increment_radiation_moment_estimators(moments, z_orig, photon.z, photon.costheta);

    if(photon.z < 0.0)
    {
      isotropic_emit_photon(&photon);
    }
    else
    {
      double xi = gsl_rand_num(0, 1);
      if(xi <= SCATTERING_ALBEDO)
      {
        isotropic_scatter_photon(&photon);
      }
      else
      {
        photon.absorb = false;
        break;
      }
    }
  }

  if(!photon.absorb)
    bin_photon_to_histogram(hist, photon.costheta);
}

/* ************************************************************************** */
/** transport_all_photons
 *
 *  @brief Control the MC interations.
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

void
transport_all_photons(char *file_name)
{
  int omp_counter = 0;
  Histogram_t hist;
  Moments_t moments;

  get_all_parameters(file_name, &hist, &moments);
  init_gsl_seed(SEED);
  init_histogram(&hist);
  init_moments(&moments);

#pragma omp parallel for \
        default(none), \
        schedule(dynamic), \
        private(packet), \
        shared(n_photons, hist, moments, output_freq, omp_counter)
  for(int i = 0; i < N_PHOTONS; i++)
  {
    transport_single_photon(&hist, &moments);

#if defined(_OPENMP)
#pragma omp atomic
    omp_counter += 1;
#pragma omp critical
{
    if (omp_counter % OUTPUT_FREQUENCY == 0)
        printf("%6.0d photon packets transported (%3.0f%%)\n", omp_counter, (double) omp_counter / N_PHOTONS * 100);
}
#else
    if((i + 1) % OUTPUT_FREQUENCY == 0)
      printf("%6.0d photon packets transported (%3.0f%%)\n", i + 1, (double) (i + 1) / N_PHOTONS * 100);
#endif
  }

  convert_weight_to_intensity(&hist);
  ouput_intensity_to_file(&hist);
  output_radiation_moments_to_file(&moments);

  free_hist(&hist);
  free_moments(&moments);
}
