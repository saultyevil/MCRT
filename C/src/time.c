/* ************************************************************************** */
/** @file time.c
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Utility functions relating to time.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <time.h>

/* ************************************************************************** */
/** print_time
 *
 *  @brief Print the current time and date to the screen.
 *
 *  @details
 *
 *  Prints the current time and date to screen using the standard C library
 *  function ctime.
 *
 * ************************************************************************** */

void
print_time(void)
{
  time_t current_time = time(NULL);
  char *c_time_string = ctime(&current_time);
  printf("Current time: %s\n", c_time_string);
}
