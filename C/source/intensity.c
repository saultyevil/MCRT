#include <stdlib.h>
#include <math.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Calculated the binned angles and initialise the bin counts as 0.
 *
 */
int init_photon_hist(Photon_hist *hist)
{
    double d_theta = 1.0/mu_bins;
    double half_width = 0.5 * d_theta;

    hist->bins = malloc(sizeof(hist->bins) * mu_bins);
    hist->theta = malloc(sizeof(hist->theta) * mu_bins);

    for (int i = 0; i < mu_bins; i++)
    {
        hist->bins[i] = 0;
        hist->theta[i] = acos(i * d_theta + half_width);
    }

    return 0;
}

/** @brief Bin a photon into the correct bin depending on its escape angle.
 *
 */
int bin_photon(Photon_hist *hist, double cos_theta)
{
    int bin_index = abs((int) (cos_theta * mu_bins));
    hist->bins[bin_index] += 1;

    return 0;
}

/** @brief Calculate the intensity of each photon bin.
 *
 */

int calculate_intensity(Photon_hist *hist, double *intensity)
{
    for (int i = 0; i < mu_bins; i++)
    {
        intensity[i] = (hist->bins[i] * mu_bins)/(2 * n_photons *
            cos(hist->theta[i]));
    }

    return 0;
}
