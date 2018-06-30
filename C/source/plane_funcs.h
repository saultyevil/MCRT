/* **************************************************************************
 *
 * Function prototypes for all of the functions in the program. Detailed
 * descriptions of the functions can be found as docstrings above the
 * function.
 *
 * ************************************************************************** */

#include <stdio.h>  // required for FILE in function prototype

/*
 * I/O
 */

int get_parameters(char *ini_file);
int get_parameters_OLD(char *ini_file);
int print_parameters(void);
int read_int(char *ini_file, char *par_name, int *parameter);
int read_double(char *ini_file, char *par_name, double *parameter);
int get_int_CL(char *par_name, int *parameter);
int get_double_CL(char *par_name, double *parameter);
int default_param_values(char *par_name, char *par_value_string, int double_sw);
int write_to_file(Photon_hist *hist, Moments *moments, double *intensity);
int print_par_header(FILE *write_file, int out_file);

/*
 * General utility
 */
int print_time(void);
double rand_num(void);
int init_seed(int seed);
double gsl_rand_num(double min, double max);
int free_mu_hist(Photon_hist *hist);
int free_moment_arrays(Moments *moments);

/*
 * Monte Carlo/transport functions
 */
int init_photon_hist(Photon_hist *hist);
int emit_photon(Photon *packet);
int start_mcrt(void);
int transport_photon_const_rho(Photon *packet, Moments *moments);
int init_jhk(Moments *moments);
int calculate_moments(Photon *packet, Moments *moments, double z2);
int isotropic_scatter(Photon *packet);
int bin_photon(Photon_hist *hist, double cos_theta);
int calculate_intensity(Photon_hist *hist, double *intensity);
