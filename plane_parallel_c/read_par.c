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
 *
 *  @param par[]: string. The name of the parameter to search for in the
 *         parameter file.
 *  @return value: double. The value of the parameter.
 */

double read_in_par(char par[])
{
    int linenum;
    double value;
    char line[MAX_LINE];
    FILE *parFile;

    if ((parFile = fopen("plane.ini", "r")) == NULL)
    {
        printf("Cannot open parameter file 'plane.ini'.\n");
        exit(-1);
    }

    linenum = 0;
    value = -1; /* set value to something to check at the end */
    while(fgets(line, MAX_LINE, parFile) != NULL)
    {
        char parName[MAX_LINE], parSeparator[MAX_LINE], parValue[MAX_LINE];
        linenum++;

        /* if the line is a comment, skip that line */
        /* bug here when there are blank lines in par file */
        if (line[0] == '#' || line[0] == ' ')
        {
            continue;
        }

        /* check a normal line is in the correct format */
        if (sscanf(line, "%s %s %s", parName, parSeparator, parValue) != 3){
            printf("Syntax error, line %d for parameter %s\n", linenum, par);
            exit(-1);
        }

        /* use strcmp to compare the difference between two strings to check
           if it's the parameter requested. */
        if (strcmp(par, parName) == 0)
        {
            value = (double) atof(parValue);
        }
    }

    /* if value wasn't updated, parameter not found. Exit the pgoram. */
    if (value == -1)
    {
        printf("Parameter %s could not be found.\nExiting simulation.\n", par);
        exit(-1);
    }

   if (fclose(parFile) != 0)
   {
    printf("File could not be closed.");
    exit(-1);
   }

    return value;
}
