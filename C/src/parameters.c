/* ************************************************************************** */
/** @file read_par.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Various functions related to reading in simulation parameters. Not
 *  the most efficient way of reading in and dealing with parameters, but
 *  is good enough. Just a bit of a pain to maintain.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** read_double
 *
 *  @brief Main control function for getting parameters from file
 *
 *  @param[in]
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

union ParameterUnion
get_single_parameter(FILE *f, char *name, int type)
{
  union ParameterUnion data;
  char line[LINE_LEN];
  char c_parameter[LINE_LEN];
  char c_value[LINE_LEN];
  char value[LINE_LEN];

  rewind(f);

  value[0] = NO_PARAMETER;
  int linenum = 0;
  while(fgets(line, LINE_LEN, f) != NULL)
  {
    linenum++;
    if(line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;

    if(sscanf(line, "%s %s", c_parameter, c_value) != 2)
    {
      printf("Syntax error: line %d for parameter %s\n", linenum, c_parameter);
      exit(1);
    }

    if(strcmp(name, c_parameter) == 0)
      strcpy(value, c_value);
  }

  if(value[0] == NO_PARAMETER)
  {
    printf("Parameter '%s' not found\n", name);
    exit(1);
  }

  if(type == TYPE_INT)
  {
    data._int = (int) strtol(value, NULL, 10);
  }
  else
  {
    data._double = strtod(value, NULL);
  }

  return data;
}

/* ************************************************************************** */
/** get_all_parameters
 *
 *  @brief Main control function for getting parameters from file
 *
 *  @param[in] *file_name  The filename of the input file.
 *
 *  @return 0
 *
 *  @details
 *
 * ************************************************************************** */

void
get_all_parameters(char *file_name, Histogram_t *hist, Moments_t *moments)
{
  FILE *f;
  if((f = fopen(file_name, "r")) == NULL)
  {
    printf("Cannot open input file %s", file_name);
    exit(1);
  }

  N_PHOTONS = (int) get_single_parameter(f, "n_photons", TYPE_DOUBLE)._double;
  OUTPUT_FREQUENCY = (int) get_single_parameter(f, "output_frequency", TYPE_DOUBLE)._double;
  SEED = get_single_parameter(f, "seed", TYPE_INT)._int;
  TAU_MAX = get_single_parameter(f, "tau_max", TYPE_DOUBLE)._double;
  SCATTERING_ALBEDO = get_single_parameter(f, "scatter_albedo", TYPE_DOUBLE)._double;

  hist->n_bins = get_single_parameter(f, "hist.n_bins", TYPE_INT)._int;
  moments->n_levels = get_single_parameter(f, "moments.n_levels", TYPE_INT)._int;

  if(fclose(f))
  {
    printf("Cannot close file %s\n", file_name);
    exit(1);
  }
}


