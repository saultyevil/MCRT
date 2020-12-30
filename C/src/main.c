/* ************************************************************************** */
/** @file main.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Contains the main function.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <time.h>

#include "variables.h"
#include "functions.h"

/* ************************************************************************** */
/** main
 *
 *  @brief The main function.
 *
 *  @param[in] int argc. Number of command line arguments provided.
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
  if(argc >= 2)
  {
    ini_file = argv[1];
  }
  else
  {
    printf("No configuration file provided. Assuming default path.\n");
    ini_file = DEFAULT_INI_FILE;
  }

  time_t start = time(NULL);
  printf("\nBeginning simulation\n");
  print_time();

  transport_all_photons(ini_file);

  time_t stop = time(NULL);
  printf("\nTotal run time %3.2f s\n", difftime(stop, start));
  printf("\n-------------\n");

  return 0;
}
