/* ************************************************************************** */
/** @file plane_funcs.h
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief
 *
 *  Function prototypes for all of the functions in the program. Detailed
 *  descriptions of the functions can be found as docstrings above the
 *  function.
 *
 * ************************************************************************** */

/*
 * default_params.c
 */
/* @brief Search for default parameter values */
int default_param_values(char *par_name, char *par_value_string, int double_sw);
/* @brief Get a default parameter value as a double */
int get_default_par_double(char *par_name, double *parameter);
/* @brief Get a default parameter value as an int */
int get_default_par_int(char *par_name, int *parameter);

/*
 * intensity.c
 */
/* @brief Bin a photon escape to a binned angle */
int bin_photon(Mu_hist *hist, double cos_theta);
/* @brief Calculate the normalised intensity of the binned angles */
int calculate_intensity(Mu_hist *hist, double *intensity);
/* @brief Initialise a Mu_hist structure */
int init_photon_hist(Mu_hist *hist);

/*
 * moments.c
 */
/* @brief Calculate the J, H, K moments at n_levels + 1 levels */
int calculate_moments(JHK_Moments *moments, double z1, double z2,
    double cos_theta);
/* @brief Intiailise a JHK_Moments structure */
int init_jhk(JHK_Moments *moments);

/*
 * MRW.c
 */
/* @brief Find the cell edge a photon is closest to */
double find_closest_cell_edge(Photon *packet);
/* @brief Transport a photon via the Modified Random Walk algorithm */
int MRW_transport(Photon *packet);

/*
 * plane.c
 */
/* @brief The main function */
int main(int argc, char *argv[]);

/*
 * random.c
 */
/* @brief Return a random double between min and max */
double gsl_rand_num(double min, double max);
/* @brief Intialise the seed for the GSL RNG */
int init_gsl_seed(int x_seed);
/* @brief Generate a random cos_theta and phi value */
int random_theta_phi(double *theta, double *phi);
/* @brief Generate a random isotropic direction */
int random_isotropic_direction(Photon *packet);
/* @brief Return a random optical depth value */
double random_tau(void);

/*
 * read_par.c
 */
/* @brief Get a double from the CL */
int get_double_CL(char *par_name, double *parameter);
/* @brief Get an int from the CL */
int get_int_CL(char *par_name, int *parameter);
/* @brief Main function for getting simulation parameters */
int get_parameters(char *ini_file);
/* @brief Print the simulation parameters */
int print_parameters(void);
/* @brief Read in a double from file */
int read_double(char *ini_file, char *par_name, double *parameter);
/* @brief Read in an int from file */
int read_int(char *ini_file, char *par_name, int *parameter);
/* @brief Write the simulation parameters to file */
int write_sim_params_file(void);

/*
 * time.c
 */
/* @brief Print the current time and date */
int print_time(void);

/*
 * transport.c
 */
/* @brief Emit a photon at the origin isotropically */
int isotropic_emit_photon(Photon *packet);
/* @brief Update a photon's position a distance ds */
int photon_pos_step(Photon *packet, double ds);
/* @brief Start the MCRT iterations */
int start_mcrt(void);
/* @brief Transport a photon through a cell of constant density */
int transport_photon_const_rho(Photon *packet, Mu_hist *hist,
    JHK_Moments *moments);

/*
 * utilities.c
 */
/* @brief Free the pointers in a JHK_Moments structure */
int free_moment_arrays(JHK_Moments *moments);
/* @brief Free the pointers in a Mu_hist structure */
int free_mu_hist(Mu_hist *hist);

/*
 * write_file.c
 */
/* @brief Write the binned angle intensity to file */
int write_intensity_to_file(Mu_hist *hist, double *intensity);
/* @brief Write the moments J, H, K to file */
int write_moments_to_file(JHK_Moments *moments);
