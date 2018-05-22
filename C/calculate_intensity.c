#include <stdlib.h>
#include <math.h>

#include "plane.h"

/** @brief Bin the angles the photon packets escape from the atmosphere at.
 *
 *  Populates the struct photon_hist, effectively creating the data required
 *  to plot a histogram of the number of photons which escaped at the binned
 *  angles.
 */

int bin_photons(struct photon_hist *histPtr, double *mu, int n_photons,
    int mu_bins)
{
    int bin_index;
    double dtheta, half_width;

    dtheta = 1.0/mu_bins;
    half_width = 0.5 * dtheta;

    /*
     * Calculate the binned angles
     */
    for (int i = 0; i < mu_bins; i++)
    {
        histPtr -> bins[i] = 0;  // set the bin count to zero
        histPtr -> theta[i] = acos(i * dtheta + half_width) * (180/M_PI);
    }

    /*
     * Bin the escape angles accordinly
     */
    for (int i = 0; i < n_photons; i++)
    {
        bin_index = abs((int)(mu[i] * mu_bins));
        histPtr -> bins[bin_index] += 1;
    }

    return 0;
}

/** @brief Calculates the intensity at the binned angles by effectively
 *         counting the number of photons which escaped at each binned angle.
 *
 *  Calculates the normalised intensity at each binned angles by effectively
 *  counting the number of photon packets which escaped at the binned angles.
 */

int calculate_intensity(double *intensity, struct photon_hist *hist,
    int n_photons, int mu_bins)
{
    for (int i = 0; i < mu_bins; i++)
    {
        intensity[i] = (hist -> bins[i] * mu_bins)/(2 * n_photons *
            cos(hist -> theta[i] * M_PI/180));
    }

    return 0;
}
