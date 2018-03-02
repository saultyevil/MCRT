#include <stdlib.h>

/* return a random number between 0 and 1 */
double random_number(void){

    return (double)rand()/(double)RAND_MAX;
}
