#include <stdlib.h>

#include "plane_variables.h"
#include "plane_functions.h"

/** @brief Free the struct type Moments and the arrays within.
 *
 */
int free_moment_arrays(Moments *moments)
{
    free(moments->j_plus);
    free(moments->h_plus);
    free(moments->k_plus);
    free(moments->j_minus);
    free(moments->h_minus);
    free(moments->k_minus);

    return 0;
}

/** @brief Free the struct type Photon_hist and the arrays within.
 *
 */
int free_mu_hist(Photon_hist *hist)
{
    free(hist->bins);
    free(hist->theta);

    return 0;
}
