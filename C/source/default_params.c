#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

/** @brief Find a default value for a parameter.
 *
 */
int default_param_values(char *par_name, char *par_value_string, int double_sw)
{
    char *params[] =
        {"MU_BINS", "N_PHOTONS", "N_LEVELS", "OUTPUT_FREQ", "TAU_MAX",
         "ALBEDO", NULL};

    char *default_values[] =
        {"30", "1000000", "10", "100000", "5.0", "1.0", NULL};

    int i = 0;
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
        printf("Cannot find default value for '%s'. Please input a value!\n",
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
