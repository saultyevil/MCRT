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
 *  @return 0
 *
 *  @details
 *
 *  Prints the current time and date to screen using the standard C library
 *  function ctime.
 *
 * ************************************************************************** */

int print_time(void)
{
    char *c_time_string;
    time_t current_time;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    printf("Current time: %s\n", c_time_string);

    return 0;
}
