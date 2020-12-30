/* ************************************************************************** */
/** @file utilities.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Functions placed in here have general uses but were a bit too messy
 *  to look at in a main program source file.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** free_moments_array
 *
 *  @brief Free the pointers within a Moments_t struct.
 *
 *  @param[in,out] *moments. An initialised Moments_t struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

void
free_moments(Moments_t *moments)
{
  moments->n_levels = 0;
  free(moments->j_plus);
  free(moments->h_plus);
  free(moments->k_plus);
  free(moments->j_minus);
  free(moments->h_minus);
  free(moments->k_minus);
}

/* ************************************************************************** */
/** free_hist
 *
 *  @brief Free the pointers within a Histogram_t struct.
 *
 *  @param[in, out] Mu_hist *hist. An initialised Histogram_t struct.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

void
free_hist(Histogram_t *hist)
{
  hist->n_bins = 0;
  free(hist->intensity);
  free(hist->weight);
  free(hist->theta);
}
