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
 *  @brief Find a default value for a parameter which is missing from the
 *  input file.
 *
 *  @param[in] char *par_name. The parameter label in the input file.
 *
 *  @param[out] char *par_value_string. Character array for storing the default
 *  value of the parameter as a string.
 *
 *  @param[in] int double_switch. Switch to indicate if the param is a double,
 *  (TRUE) or an int (FALSE).
 *
 *  @return 0
 *
 *  @details
 *
 *  The parameter label and it's corresponding default value are saved in
 *  separate arrays with the same index. The params array is iterated over in
 *  an attempt to find a match between par_name and an element within the params
 *  array. If a match is found, the default value as a string is assigned to
 *  par_value_string. If no match is found between par_name and params, then
 *  the value is taken in as user input during the program.
 *
 * ************************************************************************** */

int default_param_values(char *par_name, char *par_value_string, int double_sw)
{
    int i = 0;

    char *params[] =
        {"MU_BINS", "N_PHOTONS", "N_LEVELS", "OUTPUT_FREQ", "TAU_MAX",
         "ALBEDO", "MRW_GAMMA", "MRW_CRIT_SCAT", "SEED", NULL};

    char *default_values[] =
        {"30", "1000000", "10", "100000", "5.0", "1.0", "0.2", "300", "1337",
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
 *  @brief Find the default value for a double simulation parameter.
 *
 *  @param[in] char *par_name. Parameter label.
 *
 *  @param[in, out] double *parameter. Pointer to the simulation parameter
 *  variable.
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
 *  @param[in] char *par_name. Parameter label.
 *
 *  @param[in, out] int *parameter. Pointer to the simulation parameter
 *  variable.
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
