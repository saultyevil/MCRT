/* ************************************************************************** */
/** @file plane.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains the main function.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <time.h>

#include "plane_vars.h"
#include "plane_funcs.h"

/* ************************************************************************** */
/** main
 *
 *  @brief The main function for, mostly, controlling the flow of the program.
 *
 *  @param[in] int argc. Number of arguments provided.
 *  @param[int] char *argv[]. The arguments provided.
 *
 *  @return 0.
 *
 *  @details
 *
 *  Controls the flow of the program.
 *
 * ************************************************************************** */

int main(int argc, char *argv[])
{
    char *ini_file;
    time_t start, stop;

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
     * Get the parameters from file
     */
    get_parameters(ini_file);

    /*
     * Initialise the random seed -- using the clock for now.
     */
    init_gsl_seed(seed);

    /*
     * Start the MCRT procedure
     */
    start = time(NULL);
    start_mcrt();
    stop = time(NULL);

    printf("\nMonte Carlo run time: ~%3.2f s.\nSimulation completed.\n",
           difftime(stop, start));
    printf("\n-------------\n\n");

    return 0;
}
