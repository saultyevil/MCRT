#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Read in parameters from an input file or from the user.
 *
 */
int get_parameters(char *ini_file)
{
    if (strcmp(ini_file, "-noparf") == 0)
    {
        printf("----------------------------\n");
        printf("USING NO PARAMETER FILE MODE\n");
        printf("----------------------------\n\n");
        printf("Please input appropriate choices for parameters.\n\n");

        get_int_CL("MU_BINS", &mu_bins);
        get_int_CL("N_PHOTONS", &n_photons);
        get_int_CL("N_LEVELS", &n_levels);
        get_int_CL("OUTPUT_FREQ", &output_freq);
        get_double_CL("TAU_MAX", &tau_max);
        get_double_CL("ALBEDO", &albedo);
    }
    else
    {
        read_int(ini_file, "MU_BINS", &mu_bins);
        read_int(ini_file, "N_PHOTONS", &n_photons);
        read_int(ini_file, "N_LEVELS", &n_levels);
        read_int(ini_file, "OUTPUT_FREQ", &output_freq);
        read_double(ini_file, "TAU_MAX", &tau_max);
        read_double(ini_file, "ALBEDO", &albedo);
    }

    print_parameters();

    return 0;
}

/** @brief Print the value of the  simulation parameters.
 *
 */
int print_parameters(void)
{
    printf("\nParameters:\n");
    printf("-----------\n\n");
    printf("N_PHOTONS      : %d\n", n_photons);
    printf("MU_BINS        : %d\n", mu_bins);
    printf("N_LEVELS       : %d\n", n_levels);
    printf("OUTPUT_FREQ    : %d\n", output_freq);
    printf("TAU_MAX        : %4.3f\n", tau_max);
    printf("ALBEDO         : %4.3f\n\n", albedo);
    printf("-----------\n\n");

    return 0;
}

/** @brief Read in a double from an external file.
 *
 */
int read_double(char *ini_file, char *par_name, double *parameter)
{
    char line[MAX_LINE], ini_par_name[MAX_LINE], par_separator[MAX_LINE];
    char par_value[MAX_LINE];

    FILE *par_file_ptr;

    if ((par_file_ptr = fopen(ini_file, "r")) == NULL)
    {
        printf("Cannot open parameter file '%s'.\n", ini_file);
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file_ptr) != NULL)
    {
        linenum++;
        if (line[0] == '#' || line[0] == '\r')
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

/** @brief Read in an int from an external file.
 *
 */
int read_int(char *ini_file, char *par_name, int *parameter)
{
    char line[MAX_LINE], ini_par_name[MAX_LINE], par_separator[MAX_LINE];
    char par_value[MAX_LINE];

    FILE *par_file_ptr;

    if ((par_file_ptr = fopen(ini_file, "r")) == NULL)
    {
        printf("Cannot open parameter file '%s'.\n", ini_file);
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file_ptr) != NULL)
    {
        linenum++;
        if (line[0] == '#' || line[0] == '\r')
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
        printf("!! Parameter '%s' not found in '%s'.", par_name, ini_file);
        printf(" Looking for a default value.\n");
        char par_default[MAX_LINE];
        default_param_values(par_name, par_default, FALSE);
        *parameter = atoi(par_default);
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

/** @brief Read in a double parameter from the command line.
 *
 */
int get_double_CL(char *par_name, double *parameter)
{
    char par_input[MAX_LINE];

    printf("DOUBLE: %s\n", par_name);
    scanf("%s", par_input);
    *parameter = (double) atof(par_input);

    return 0;
}

/** @brief Read in an int parameter from the command line.
 *
 */
int get_int_CL(char *par_name, int *parameter)
{
    char par_input[MAX_LINE];

    printf("INT: %s\n", par_name);
    scanf("%s", par_input);
    *parameter = atoi(par_input);

    return 0;
}

/** @brief Read in the simulation parameters from file.
 *
 */
int get_parameters_OLD(char *ini_file)
{
    int no_par_flag = FALSE;
    char line[MAX_LINE], par_name[MAX_LINE], separator[MAX_LINE];
    char par_value[MAX_LINE];

    /*
     * Set default values for the parameters to read in from file
     */
    mu_bins = 10;
    n_photons = 1000000;
    n_levels = 10;
    output_freq = 100000;
    tau_max = 5.0;
    albedo = 1.0;

    FILE *par_file_ptr;

    if ((par_file_ptr = fopen(ini_file, "r")) == NULL)
    {
        printf("Cannot open parameter file '%s'.\n", ini_file);
        exit(-1);
    }

    int line_num = 0;

    printf("Parameters:\n");
    printf("-----------\n\n");

    while (fgets(line, MAX_LINE, par_file_ptr) != NULL)
    {
        line_num += 1;

        if (line[0] == '#' || line[0] == '\r')
        {
            continue;
        }

        if (sscanf(line, "%s %s %s", par_name, separator, par_value) != 3)
        {
            printf("\n---------------------\n");
            printf("Snytax error: line %d.\n", line_num);
            printf("---------------------\n\n");
            continue;
        }

        if (strcmp(par_name, "N_PHOTONS") == 0)
        {
            n_photons = atoi(par_value);
        }
        else if (strcmp(par_name, "MU_BINS") == 0)
        {
            mu_bins = atoi(par_value);
        }
        else if (strcmp(par_name, "N_LEVELS") == 0)
        {
            n_levels = atoi(par_value);
        }
        else if (strcmp(par_name, "OUTPUT_FREQ") == 0)
        {
            output_freq = atoi(par_value);
        }
        else if (strcmp(par_name, "TAU_MAX") == 0)
        {
            tau_max = (double) atof(par_value);
        }
        else if (strcmp(par_name, "ALBEDO") == 0)
        {
            albedo = (double) atof(par_value);
        }
        else
        {
            printf("Parameter '%s' on line %d not recognised.\n",
                par_name, line_num);
            no_par_flag = TRUE;
        }

        if (no_par_flag == FALSE)
            printf("%s  ::  %s\n", par_name, par_value);
    }

    printf("\n-----------\n\n");

    if (fclose(par_file_ptr) != 0)
    {
        printf("Cannot close parameter file '%s'.\n", ini_file);
        exit(-1);
    }

    return 0;
}
