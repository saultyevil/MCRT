#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "plane_Variables.h"
#include "plane_Functions.h"

int main(int argc, char *argv[])
{
    char *ini_file;

    printf("\nBeginning simulation...\n");
    print_time();

    if (argc >= 2)
    {
        ini_file = argv[1];
    }
    else
    {
        printf("No configuration file provided in the program arguments.\n");
        printf("Using default configuration file: %s.\n\n", DEFAULT_INI_FILE);
        ini_file = DEFAULT_INI_FILE;
    }

    time_t start = time(NULL);

    /*
     * Initialise the random seed using the clock for now.
     */
    init_seed(time(NULL));

    /*
     * Get the simulation parameters from file and start the MCRT procedure
     */
    get_parameters(ini_file);
    start_mcrt();

    time_t stop = time(NULL);

    printf("\nSimulation completed.\nTime elapsed: %3.2f s.\n",
           difftime(stop, start));

    return 0;
}
