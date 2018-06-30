#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <omp.h>

#include "plane_vars.h"
#include "plane_funcs.h"

int main(int argc, char *argv[])
{
    char *ini_file;
    time_t start,
           stop;



    printf("\nBeginning simulation...\n");
    print_time();

    if (argc >= 2)
    {
        ini_file = argv[1];
    }
    else
    {
        printf("No configuration file option provided in program argument.\n");
        printf("Using default configuration filename: %s.\n\n",
               DEFAULT_INI_FILE);
        ini_file = DEFAULT_INI_FILE;
    }

    /*
     * Initialise the random seed -- using the clock for now.
     */
    init_seed(time(NULL));

    /*
     * Get the simulation parameters from file and start the MCRT procedure
     */
    get_parameters(ini_file);

    start = time(NULL);
    start_mcrt();
    stop = time(NULL);

    printf("\nSimulation completed.\nMonte Carlo run time: ~%3.2f s.\n\n",
           difftime(stop, start));

    return 0;
}
