#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plane.h"

/** @brief Read in parameters from an external file.
 *
 *  Opens a file named 'mrw.ini' and reads in each line looking for a parameter
 *  name which is the same as the input par[]. If the parameter is found, the
 *  value of the parameter will be returned. If the file cannot be opened or
 *  closed, or if the syntax of the ini file is incorrect the program will
 *  exit.
 */

int read_double(char *par_string, double *parameter)
{
    char line[MAX_LINE], ini_par_name[MAX_LINE], par_separator[MAX_LINE];
    char par_value[MAX_LINE];

    FILE *par_file;

    if ((par_file = fopen(INI_FILE, "r")) == NULL)
    {
        printf("Cannot open parameter file '%s'.\n", INI_FILE);
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file) != NULL)
    {
        linenum++;

        /*
         * If the line is a comment, skip that line. Note: There is a bug here
         * which will cause the script to crash if there is a blank line.
         */
        if (line[0] == '#' || line[0] == ' ')
        {
            continue;
        }

        /*
         * Check a normal line is in the correct format
         */
        if (sscanf(line, "%s %s %s", ini_par_name, par_separator, par_value) \
            != 3)
        {
            printf("Syntax error, line %d for parameter %s\n", linenum,
                   par_string);
            exit(-1);
        }

        /*
         * Use strcmp to compare the difference between two strings. If it's
         * the same parameter, then strcmp will return 0.
         */
        if (strcmp(par_string, ini_par_name) == 0)
        {
            *parameter = (double) atof(par_value);
        }
    }

    /*
     * If parameter wasn't updated, the parameter was not found. Return an
     * error.
     */
    if (*parameter == NO_PAR_CONST)
    {
        printf("Parameter %s could not be found.\nExiting simulation.\n",
               par_string);
        exit(-1);
    }

   if (fclose(par_file) != 0)
   {
    printf("File could not be closed.\n");
    exit(-1);
   }

    return 0;
}

int read_long(char *par_string, long *parameter)
{
    char line[MAX_LINE], ini_par_name[MAX_LINE], par_separator[MAX_LINE];
    char par_value[MAX_LINE];

    FILE *par_file;

    if ((par_file = fopen(INI_FILE, "r")) == NULL)
    {
        printf("Cannot open parameter file '%s'.\n", INI_FILE);
        exit(-1);
    }

    int linenum = 0;
    *parameter = NO_PAR_CONST;

    while(fgets(line, MAX_LINE, par_file) != NULL)
    {
        linenum++;

        /*
         * If the line is a comment, skip that line. Note: There is a bug here
         * which will cause the script to crash if there is a blank line.
         */
        if (line[0] == '#' || line[0] == ' ')
        {
            continue;
        }

        /*
         * Check a normal line is in the correct format
         */
        if (sscanf(line, "%s %s %s", ini_par_name, par_separator, par_value) \
            != 3)
        {
            printf("Syntax error, line %d for parameter %s\n", linenum,
                   par_string);
            exit(-1);
        }

        /*
         * Use strcmp to compare the difference between two strings. If it's
         * the same parameter, then strcmp will return 0.
         */
        if (strcmp(par_string, ini_par_name) == 0)
        {
            *parameter = (long) atoi(par_value);
        }
    }

    /*
     * If parameter wasn't updated, the parameter was not found. Return an
     * error.
     */
    if (*parameter == NO_PAR_CONST)
    {
        printf("Parameter %s could not be found.\nExiting simulation.\n",
               par_string);
        exit(-1);
    }

   if (fclose(par_file) != 0)
   {
    printf("File could not be closed.\n");
    exit(-1);
   }

    return 0;
}
