/* ************************************************************************** */
/** @file utilities.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Functions placed in here have general uses but were a bit too messy
 *  to look at in a main program source file.
 *
 * ************************************************************************** */

#include <stdlib.h>

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** free_moments_array
 *
 *  @brief Free the pointers within a Moments struct.
 *
 *  @param[in, out] Moments *moments. An initialised Moments struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

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

/* ************************************************************************** */
/** free_mu_hist
 *
 *  @brief Free the pointers within a Mu_hist struct.
 *
 *  @param[in, out] Mu_hist *hist. An initialised Mu_hist struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

int free_mu_hist(Mu_hist *hist)
{
    free(hist->bins);
    free(hist->theta);

    return 0;
}
