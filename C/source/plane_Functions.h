/* **************************************************************************
 *
 * Function prototypes for all of the functions in the program. Detailed
 * descriptions of the functions can be found as docstrings above the
 * function.
 *
 * ************************************************************************** */

/*
 * I/O
 */
int get_parameters(char *ini_file);
int read_double(char *par_string, double *parameter, char *ini_file);
int read_long(char *par_string, long *parameter, char *ini_file);
int read_int(char *par_string, int *parameter, char *ini_file);
int write_to_file(Photon_hist *hist, double *intensity);

/*
 * General utility
 */
int print_time(void);
double rand_num(void);
int init_seed(int seed);
double gsl_rand_num(double min, double max);

/*
 * Monte Carlo/transport functions
 */
int init_photon_hist(Photon_hist *hist);
int emit_photon(Photon *packet);
int start_mcrt(void);
int transport_photon_const_rho(Photon *packet);
int isotropic_scatter(Photon *packet);
int bin_photon(Photon_hist *hist, double cos_theta);
int calculate_intensity(Photon_hist *hist, double *intensity);
