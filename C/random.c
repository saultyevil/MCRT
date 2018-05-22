#include <math.h>
#include <stdlib.h>

/** @brief Returns a random number between 0 and 1.
 *
 *  Uses the standard library rand() function to generate random numbers between
 *  0 and 1.
 */

double random_number(void)
{
    return (double)rand()/(double)RAND_MAX;
}
