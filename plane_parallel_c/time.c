#include <stdio.h>
#include <time.h>

/** @brief Prints the date and time to the screen.
 *
 *  Uses the time and ctime functions to get the current date and time. They are
 *  then printed to the screen.
 *
 *  @param Void.
 *  @return 0: int, if all goes well!
 */

int print_time(void)
{
    char* c_time_string;
    time_t current_time;

    /* get the time and do some ctime magic */
    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    printf("Current time: %s\n", c_time_string);

    return 0;
}
