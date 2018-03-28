#include <stdlib.h>
#include <math.h>

#include <stdio.h>

#include "plane.h"

/** @brief Bin the angles the photon packets escape from the atmosphere at.
 *
 *  Populates the struct photon_hist, effectively creating the data required
 *  to plot a histogram of the number of photons which escaped at the binned
 *  angles.
 *
 *  @param mu[]: array of doulbes. The escape angle of each photon packet.
 *         n_photons: int. The number of photon packets used.
 *         mu_bins: int. The number of bins to bin the escape angles.
 *  @return histPtr: struct. The address in memory of where the histPtr struct
 *          is kept.
 */

struct photon_hist *bin_photons(double mu[], int n_photons, int mu_bins)
{
    int bin_index;
    double dtheta, half_width;

    struct photon_hist *histPtr = malloc(sizeof(struct photon_hist));

    dtheta = 1.0/mu_bins;
    half_width = 0.5 * dtheta;

    /* calculate the binned angles */
    for (int i = 0; i < mu_bins; i++)
    {
        histPtr -> bins[i] = 0;  /* set the bin count to zero */
        histPtr -> theta[i] = acos(i * dtheta + half_width) * (180/M_PI);
    }

    /* bin the escape angles accordinly */
    for (int i = 0; i < n_photons; i++)
    {
        bin_index = abs((int)(mu[i] * mu_bins));
        histPtr -> bins[bin_index] += 1;
    }

    return histPtr;
}

/** @brief Calculates the intensity at the binned angles by effectively
 *         counting the number of photons which escaped at each binned angle.
 *
 *  Calculates the normalised intensity at each binned angles by effectively
 *  counting the number of photon packets which escaped at the binned angles.
 *
 *  @param *hist: Pointer to struct containing the binned angles and counts.
 *         *intensity: Pointer to an array for storing the intensity.
 *         n_photons: int. The number of photon packets.
 *         mu_bins: int. The number of binned angles.
 *  @return 0, if all goes well.
 */

double *calculate_intensity(struct photon_hist *hist, int n_photons,
int mu_bins)
{
    double *intensity = malloc(sizeof(double) * mu_bins);

    for (int i = 0; i < mu_bins; i++)
    {
        intensity[i] = (hist -> bins[i] * mu_bins)/(2 * n_photons *
            cos(hist -> theta[i] * M_PI/180));
    }

    return intensity;
}
