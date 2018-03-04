#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plane.h"

double read_in_par(char par[]){
    int linenum;
    double value;
    char line[MAX_LINE];
    FILE *parFile;

    if ((parFile = fopen("plane.ini", "r")) == NULL){
        printf("Cannot open parameter file 'plane.ini'.\n");
        return -1;
    }

    linenum = 0;
    while(fgets(line, MAX_LINE, parFile) != NULL){
        char parName[MAX_LINE], parSeparator[MAX_LINE], parValue[MAX_LINE];
        linenum++;

        /* if the line is a comment, skip that line */
        if (line[0] == '#'){
            continue;
        }

        /* check a normal line is in the correct format */
        if (sscanf(line, "%s %s %s", parName, parSeparator, parValue) != 3){
            printf("Syntax error, line %d for parameter %s\n", linenum, par);

            return -1;
        }

        if (strcmp(par, parName) == 0){
            value = (double) atof(parValue);
        }
    }

   if (fclose(parFile) != 0){
    printf("File could not be closed.");
    return -1;
    }

    return value;
}