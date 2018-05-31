#include <math.h>
#include <stdlib.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

gsl_rng *rng;

/** @brief Generate a random number -- rather poorly.
 *
 */

double rand_num(void)
{
    return (double)rand()/(double)RAND_MAX;
}

/*
 * I stole these two functions from PYTHON :^).
 */

int init_seed(int seed)
{
    rng = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(rng, seed);

    return 0;
}

double gsl_rand_num(double min, double max)
{
    double num = gsl_rng_uniform_pos(rng);

    return min + ((max - min) * num);
}