/* ************************************************************************** */
/** @file default_params.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains function(s) for obtaining default values for pamameters.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** default_param_values
 *
 *  @brief Find a default value for a parameter which is meant to be read in
 *  from the parameter file.
 *
 *  @param[in] char *par_name. The string for the parameter label
 *  @param[in] int double_switch. Switch to indicate if the param is a float.
 *  The allowed values are either true or false.
 *  @param[out] char *par_value_string. The parameter value as a string.
 *
 *  @return 0
 *
 *  @details
 *
 *  Looks up the values of the parameter in two arrays, one containing the name
 *  of the parameter and the other containing the default value. If a default
 *  value isn't found, i.e. I forgot to add it to the arrays, then the user
 *  will be prompted to input a value.
 *
 * ************************************************************************** */

int default_param_values(char *par_name, char *par_value_string, int double_sw)
{
    int i = 0;

    char *params[] =
        {"MU_BINS",
         "N_PHOTONS",
         "N_LEVELS",
         "OUTPUT_FREQ",
         "TAU_MAX",
         "ALBEDO",
         "MRW_GAMMA",
         "MRW_CRIT_SCAT",
         "SEED",
         NULL};

    char *default_values[] =
        {"30",
         "1000000",
         "10",
         "100000",
         "5.0",
         "1.0",
         "0.2",
         "50",
         "1337",
         NULL};

    par_value_string[0] = NO_PAR_CONST_STRING;

    while (params[i] != NULL)
    {
        if (strcmp(par_name, params[i]) == 0)
        {
            strcpy(par_value_string, default_values[i]);
            break;
        }

        i += 1;
    }

    if (par_value_string[0] == NO_PAR_CONST_STRING)
    {
        printf("Cannot find default value for '%s'. Please input a value.\n",
               par_name);

        switch (double_sw)
        {
            case FALSE:
                printf("INT: %s\n", par_name);
                scanf("%s", par_value_string);
                break;
            case TRUE:
                printf("DOUBLE: %s\n", par_name);
                scanf("%s", par_value_string);
                break;
        }
    }

    return 0;
}

/* ************************************************************************** */
/** get_def_par_double
 *
 *  @brief Find the default value for a double simulation paramaeter.
 *
 *  @param[in] char *par_name. The parameter label.
 *  @param[in, out] double *parameter. The simulation parameter to apply.
 *
 *  @return 0
 *
 *  @details
 *
 *  Searches default parameters array to find a default value that the parameter
 *  given the variable par_name which is the label for the parameter. If a
 *  parameter label, or default value, for par_name can't be found, then the
 *  program will prompt the user to input a value.
 *
 * ************************************************************************** */

int get_default_par_double(char *par_name, double *parameter)
{
    char par_value_str[MAX_LINE];

    default_param_values(par_name, par_value_str, TRUE);
    *parameter = (double) atof(par_value_str);

    return 0;
}

/* ************************************************************************** */
/**   get_def_par_int
 *
 *  @brief Find the default value for an int simulation paramaeter.
 *
 *  @param[in] char *par_name. The parameter label.
 *  @param[in, out] int *parameter. The simulation parameter to apply.
 *
 *  @return 0
 *
 *  @details
 *
 *  Searches default parameters array to find a default value that the parameter
 *  given the variable par_name which is the label for the parameter. If a
 *  parameter label, or default value, for par_name can't be found, then the
 *  program will prompt the user to input a value.
 *
 * ************************************************************************** */

int get_default_par_int(char *par_name, int *parameter)
{
    char par_value_str[MAX_LINE];

    default_param_values(par_name, par_value_str, FALSE);
    *parameter = atoi(par_value_str);

    return 0;
}
