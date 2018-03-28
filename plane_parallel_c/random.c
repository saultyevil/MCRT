#include <stdlib.h>

/** @brief Returns a random number between 0 and 1.
 *
 *  Uses the standard library rand() function to generate random numbers between
 *  0 and 1.
 *
 *  @param Void.
 *  @return A random number between 0 and 1: double.
 */

double random_number(void)
{
    return (double)rand()/(double)RAND_MAX;
}
