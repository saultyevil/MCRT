/* ************************************************************************** */
/** @file read_par.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Various functions related to reading in simulation parameters.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#if defined(_OPENMP)
    #include <omp.h>
#endif

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** print_parameters
 *
 *  @brief Print the simulation parameters in use to screen.
 *
 *  @return 0
 *
 *  @details
 *
 *  Prints all of the global simulation parameters to the screen as a little
 *  bit of information to the user.
 *
 * ************************************************************************** */

int print_parameters(void)
{
    printf("-------------\n");
    printf(" Parameters:\n");
    printf("-------------\n\n");
    printf("N_PHOTONS      :: %d\n", n_photons);
    printf("MU_BINS        :: %d\n", mu_bins);
    printf("N_LEVELS       :: %d\n", n_levels);
    printf("SEED           :: %i\n", seed);
    printf("OUTPUT_FREQ    :: %d\n", output_freq);
    printf("TAU_MAX        :: %4.3f\n", tau_max);
    printf("ALBEDO         :: %4.3f\n", albedo);

    #if defined(_OPENMP)
        int n_threads = omp_get_max_threads();
        printf("\nN_OMP_THREADS  :: %d\n", n_threads);
    #endif

    printf("\n-------------\n\n");

    return 0;
}

/* ************************************************************************** */
/** write_sim_params_file
 *
 *  @brief Write the chosen simulation parameters to file.
 *
 *  @return 0
 *
 *  @details
 *
 *  Write all of the global simulation parameters to file, possibly for later
 *  use as the file is formatted such that the program should be able to read in
 *  the parameters from this file.
 *
 * ************************************************************************** */

int write_sim_params_file(void)
{
    FILE *write_file_parameters = NULL;

    if ((write_file_parameters = fopen(OUTPUT_FILE_PARS, "w")) == NULL)
    {
        printf("Cannot open file '%s' for writing.\n", OUTPUT_FILE_PARS);
        exit(-1);
    }

    fprintf(write_file_parameters, "# -------------\n");
    fprintf(write_file_parameters, "# Parameters:\n");
    fprintf(write_file_parameters, "# -------------\n\n");
    fprintf(write_file_parameters, "N_PHOTONS      :: %d\n", n_photons);
    fprintf(write_file_parameters, "MU_BINS        :: %d\n", mu_bins);
    fprintf(write_file_parameters, "N_LEVELS       :: %d\n", n_levels);
    fprintf(write_file_parameters, "SEED           :: %d\n", seed);
    fprintf(write_file_parameters, "OUTPUT_FREQ    :: %d\n", output_freq);
    fprintf(write_file_parameters, "TAU_MAX        :: %4.3f\n", tau_max);
    fprintf(write_file_parameters, "ALBEDO         :: %4.3f\n", albedo);
    fprintf(write_file_parameters,"\n# -------------\n\n");

    if (fclose(write_file_parameters) != 0)
    {
        printf("Cannot close file '%s'.\n", OUTPUT_FILE_PARS);
        exit(-1);
    }

    return 0;
}

/* ************************************************************************** */
/** read_double
 *
 *  @brief Read in a double from file.
 *
 *  @param[in] char *ini_file. The filename to read from.
 *
 *  @param[in] char *par_name. The label given to a parameter in the file.
 *
 *  @param[in, out] double *parameter. A pointer to the parameter being searched
 *  for in the parameter file.
 *
 *  @return 0
 *
 *  @details
 *
 *  Iterates through the file looking for a label consistent with par_name. Once
 *  this is found, the variable parameter is updated with the value which is
 *  found in the input file. If no value can be found for it, the function will
 *  look for a default value.
 *
 * ************************************************************************** */

int read_double(char *ini_file, char *par_name, double *parameter)
{
    char line[MAX_LINE],
         ini_par_name[MAX_LINE],
         par_separator[MAX_LINE],
         par_value[MAX_LINE];

    FILE *par_file_ptr;

    if ((par_file_ptr = fopen(ini_file, "r")) == NULL)
    {
        printf("!! The provided input file '%s' cannot be opened, it doesn't ",
               ini_file);
        printf("exist or read access has been denied.\n\n");
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file_ptr) != NULL)
    {
        linenum++;
        if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
            continue;

        if (sscanf(line, "%s %s %s", ini_par_name, par_separator, par_value) \
            != 3)
        {
            printf("Syntax error: line %d for parameter %s\n", linenum,
                   par_name);
            exit(-1);
        }

        if (strcmp(par_name, ini_par_name) == 0)
            *parameter =  (double) atof(par_value);
    }

    /*
     * If parameter wasn't updated, the parameter was not found in the file so
     * look up a default value to use
     */
    if (*parameter == NO_PAR_CONST)
    {
        printf("!! Parameter '%s' not found in '%s'.", par_name, ini_file);
        printf(" Looking for a default value.\n");
        char par_default[MAX_LINE];
        default_param_values(par_name, par_default, TRUE);
        *parameter = (double) atof(par_default);
        printf("!! Default value of '%s' found for parameter '%s'.\n\n",
               par_default, par_name);
    }

   if (fclose(par_file_ptr) != 0)
   {
        printf("Parameter file '%s' could not be closed.\n", ini_file);
        exit(-1);
   }

    return 0;
}

/* ************************************************************************** */
/** read_int
 *
 *  @brief Read in an int from file.
 *
 *  @param[in] char *ini_file. The filename to read from.
 *
 *  @param[in] char *par_name. The label given to a parameter in the file.
 *
 *  @param[in, out] int *parameter. A pointer to the parameter being searched
 *  for in the parameter file.
 *
 *  @return 0
 *
 *  @details
 *
 *  Iterates through the file looking for a label consistent with par_name. Once
 *  this is found, the variable parameter is updated with the value which is
 *  found in the input file. If no value can be found for it, the function will
 *  look for a default value.
 *
 *  For the input parameter SEED, i.e. the seed for RNG, is not specified.
 *  Instead of looking up a default parameter value, we instead will use
 *  whatever the C library time function returns.
 *
 * ************************************************************************** */

int read_int(char *ini_file, char *par_name, int *parameter)
{
    char line[MAX_LINE],
         ini_par_name[MAX_LINE],
         par_separator[MAX_LINE],
         par_value[MAX_LINE];

    FILE *par_file_ptr;

    if ((par_file_ptr = fopen(ini_file, "r")) == NULL)
    {
        printf("!! The provided input file '%s' cannot be opened, it doesn't ",
               ini_file);
        printf("exist or read access has been denied.\n\n");
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file_ptr) != NULL)
    {
        linenum++;
        if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
            continue;

        if (sscanf(line, "%s %s %s", ini_par_name, par_separator, par_value) \
            != 3)
        {
            printf("Syntax error: line %d for parameter %s\n", linenum,
                   par_name);
            exit(-1);
        }

        if (strcmp(par_name, ini_par_name) == 0)
            *parameter =  atoi(par_value);
    }

    /*
     * If parameter wasn't updated, the parameter was not found in the file so
     * look up a default value to use
     */
    if (*parameter == NO_PAR_CONST)
    {
        if (strcmp(par_name, "SEED") == 0)
        {
            printf("!! No seed provided, using UNIX time as seed.\n\n");
            *parameter = time(NULL);
        }
        else
        {
            char par_default[MAX_LINE];

            printf("!! Parameter '%s' not found in '%s'.", par_name, ini_file);
            printf(" Looking for a default value.\n");

            default_param_values(par_name, par_default, FALSE);
            *parameter = atoi(par_default);

            printf("!! Default value of '%s' found for parameter '%s'.\n\n",
                par_default, par_name);
        }
    }

   if (fclose(par_file_ptr) != 0)
   {
        printf("Parameter file '%s' could not be closed.\n", ini_file);
        exit(-1);
   }

    return 0;
}

/* ************************************************************************** */
/** get_double_CL
 *
 *  @brief Take in a double as input from the command line.
 *
 *  @param[in] char *par_name. The label for the parameter.
 *
 *  @param[in, out] double *parameter. A pointer to the parameter which is being
 *  input.
 *
 *  @return 0
 *
 *  @details
 *
 *  Used in -noparf mode or when no default can be found. Prompts the user to
 *  input a value for the parameter in question.
 *
 * ************************************************************************** */

int get_double_CL(char *par_name, double *parameter)
{
    char par_input[MAX_LINE];

    printf("DOUBLE: %s\n", par_name);
    scanf("%s", par_input);
    *parameter = (double) atof(par_input);

    return 0;
}

/* ************************************************************************** */
/** get_int_CL
 *
 *  @brief Take in an int as input from the command line.
 *
 *  @param[in] char *par_name. The label for the parameter.
 *
 *  @param[in, out] int *parameter. A pointer to the parameter which is being
 *  input.
 *
 *  @return 0
 *
 *  @details
 *
 *  Used in -noparf mode or when no default can be found. Prompts the user to
 *  input a value for the parameter in question.
 *
 * ************************************************************************** */

int get_int_CL(char *par_name, int *parameter)
{
    char par_input[MAX_LINE];

    printf("INT: %s\n", par_name);
    scanf("%s", par_input);
    *parameter = atoi(par_input);

    return 0;
}

/* ************************************************************************** */
/** get_parameters
 *
 *  @brief Main control function for getting parameters from file
 *
 *  @param[in] char *ini_file. The filename of the input file.
 *
 *  @return 0
 *
 *  @details
 *
 *  If -noparf is provided as the intput filename, then the function will
 *  attempt to recieve the parameters from command line input. Otherwise, if a
 *  parameter filename is provided, the function will call functions to search
 *  that file for parameters. The parameters will then be printed to screen and
 *  written to file.
 *
 * ************************************************************************** */

int get_parameters(char *ini_file)
{
    if (strcmp(ini_file, "-noparf") == 0)
    {
        printf("----------------------------\n");
        printf("   NO PARAMETER FILE MODE\n");
        printf("----------------------------\n\n");
        printf("Please input appropriate choices for parameters.\n\n");

        get_int_CL("MU_BINS", &mu_bins);
        get_int_CL("N_PHOTONS", &n_photons);
        get_int_CL("N_LEVELS", &n_levels);
        get_int_CL("OUTPUT_FREQ", &output_freq);
        get_int_CL("SEED", &seed);
        get_double_CL("TAU_MAX", &tau_max);
        get_double_CL("ALBEDO", &albedo);
    }
    else if (strcmp(ini_file, "-default_pars") == 0)
    {
        get_default_par_int("MU_BINS", &mu_bins);
        get_default_par_int("N_PHOTONS", &n_photons);
        get_default_par_int("N_LEVELS", &n_levels);
        get_default_par_int("OUTPUT_FREQ", &output_freq);
        get_default_par_int("SEED", &seed);
        get_default_par_double("TAU_MAX", &tau_max);
        get_default_par_double("ALBEDO", &albedo);
    }
    else
    {
        read_int(ini_file, "MU_BINS", &mu_bins);
        read_int(ini_file, "N_PHOTONS", &n_photons);
        read_int(ini_file, "N_LEVELS", &n_levels);
        read_int(ini_file, "OUTPUT_FREQ", &output_freq);
        read_int(ini_file, "SEED", &seed);
        read_double(ini_file, "TAU_MAX", &tau_max);
        read_double(ini_file, "ALBEDO", &albedo);
    }

    print_parameters();
    write_sim_params_file();

    return 0;
}
