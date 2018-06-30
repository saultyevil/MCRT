#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "plane_variables.h"
#include "plane_functions.h"

/** @brief Calculate the photon's contribution to the moments of the radiation
 *         field.
 *
 */
int calculate_moments(Photon *packet, Moments *moments, double z2)
{
    int level1 = 0;
    int level2 = 0;
    double z1 = packet->z;

    if ((z1 > 0) && (z2 > 0) && ((int) (z1*n_levels) == (int) (z2*n_levels)))
        return 0;

    if (packet->cos_theta > 0)
    {
        if (z1 < 0)
            level1 = 1;
        else
            level1 = (int) (z1 * n_levels) + 2;
        if (z2 > 1)
            level2 = n_levels + 1;
        else
            level2 = (int) (z2 * n_levels) + 1;

        // printf("up: level1\t %d\t level2\t %d\n", level1, level2);
        for (int i = level1; i < level1; i++)
        {
            moments->j_plus[i] += 1/packet->cos_theta;
            moments->h_plus[i] += 1;
            moments->k_plus[i] += packet->cos_theta;
        }
    }
    else if (packet->cos_theta < 0)
    {
        level1 = (int) (z1 * n_levels) + 1;

        if (z2 < 0)
            level2 = 1;
        else
            level2 = (int) (z2 * n_levels) + 2;

        // printf("do: level1\t %d\t level2\t %d\n", level1, level2);
        for (int i = level2; i < level1; i++)
        {
            moments->j_minus[i] += 1/fabs(packet->cos_theta);
            moments->h_minus[i] -= 1;
            moments->k_minus[i] += fabs(packet->cos_theta);
        }
    }

    return 0;
}

/** @brief Initialise the moments array for the Moments data type.
 *
 */
int init_jhk(Moments *moments)
{

    moments->j_plus = malloc(sizeof(*moments->j_plus) * n_levels);
    moments->h_plus = malloc(sizeof(*moments->h_plus) * n_levels);
    moments->k_plus = malloc(sizeof(*moments->k_plus) * n_levels);
    moments->j_minus = malloc(sizeof(*moments->j_minus) * n_levels);
    moments->h_minus = malloc(sizeof(*moments->h_minus) * n_levels);
    moments->k_minus = malloc(sizeof(*moments->k_minus) * n_levels);

    for (int i = 0; i < n_levels; i++)
    {
        moments->j_plus[i] = 0.0;
        moments->h_plus[i] = 0.0;
        moments->k_plus[i] = 0.0;
        moments->j_minus[i] = 0.0;
        moments->h_minus[i] = 0.0;
        moments->k_minus[i] = 0.0;
    }

    return 0;
}
