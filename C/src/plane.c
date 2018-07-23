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
 *  @brief The main function.
 *
 *  @param[in] int argc. Number of command line arguments provided.
 *
 *  @param[int] char *argv[]. The command line arguments provided.
 *
 *  @return 0
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

    if (argc >= 2)
    {
        ini_file = argv[1];
    }
    else
    {
        printf("No configuration file provided. Assuming default path.\n");
        ini_file = DEFAULT_INI_FILE;
    }

    start = time(NULL);

    printf("\nBeginning simulation...\n");
    print_time();
    get_parameters(ini_file);
    init_gsl_seed(seed);
    start_mcrt(); // start the MCRT simulation iterations

    stop = time(NULL);

    printf("\nMonte Carlo run time: ~%3.2f s.\nSimulation completed.\n",
           difftime(stop, start));
    printf("\n-------------\n\n");

    return 0;
}
