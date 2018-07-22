/* ************************************************************************** */
/** @file plane_vars.h
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Useful variables and data types/structures used throughout the code.
 *
 *  @typedef plane_vars::Photon
 *  Struct used to represent a photon packet in the simulation.
 *  @typedef plane_vars::Mu_hist
 *  Struct used to store the escape angle bins.
 *  @typedef plane_vars::JHK_Moments
 *  Struct used to store the plus and minus moments of the radiation
 *  field.
 *
 * ************************************************************************** */

/* ************************************************************************** */
/**
 *  @def FALSE
 *  False = 0
 *  @def TRUE
 *  True = 1
 *
 *  @def PI
 *  The value of pi to many decimal places.
 *  @def RAD_TO_DEG
 *  Used to convert radians to degrees.
 *  @def DEG_TO_RAD
 *  Used to convert degrees to radians.
 *
 *  @def MAX_LINE
 *  The maximum number of characters a line read in file can be.
 *  @def NO_PAR_CONST
 *  A value used to determine if a parameter has been found in a file or not.
 *  @def NO_PAR_CONST_STRING
 *  A character used to determine if a parameter has been found in a file or
 *  not.
 *  @def DEFAULT_INI_FILE
 *  The filename for the default parameter input file.
 *  @def OUTPUT_FILE_INTENS
 *  The default filename for the output intensity file.
 *  @def OUTPUT_FILE_MOMENTS
 *  The default filename for the output moments file.
 *  @def OUTPUT_FILE_PARS
 *  The default filename for the output simulation parameters file.
 *
 * ************************************************************************** */

#define FALSE 0
#define TRUE 1

/*
 * Makes converting between radians and degrees easier
 */
#define PI 3.1415926535897932
#define RAD_TO_DEG 180.0/PI
#define DEG_TO_RAD PI/180.0

/*
 * Constants for I/O
 */
#define MAX_LINE 128
#define NO_PAR_CONST -1
#define NO_PAR_CONST_STRING '\0'
#define DEFAULT_INI_FILE "mrw.ini"
#define OUTPUT_FILE_INTENS "binned_intens.txt"
#define OUTPUT_FILE_MOMENTS "moments.txt"
#define OUTPUT_FILE_PARS "sim_params.txt"

/* ************************************************************************** */
/**
 *  Global variables
 *
 *  @var plane_vars::mu_bins
 *  The number of bins to be used for binning the escape angles.
 *  Input label "MU_BINS"
 *  @var plane_vars::n_photons
 *  The number of Monte Carlo MCRT iterations. Physically, this is the number
 *  of photons which will be transported.
 *  Input label "N_PHOTONS"
 *  @var plane_vars::n_levels
 *  The number of levels used to calculated the moments of the radiation field.
 *  Input label "N_LEVELS"
 *  @var plane_vars::output_freq
 *  The progress bar output frequency.
 *  Input label "OUTPUT_FREQ"
 *  @var plane_vars::seed
 *  The seed used for random number generation.
 *  Input label "SEED"
 *  @var plane_vars::tau_max
 *  The maximum optical depth a photon can traverse.
 *  Input label "TAU_MAX"
 *  @var plane_vars::albedo
 *  The scattering albedo for photon interactions.
 *  Input label "ALBEDO"
 *  @var plane_vars::mrw_critical_scatters
 *  The number of scatterings a photon can undergo before transported via the
 *  Modified Random Walk is possible.
 *  Input label "MRW_CRIT_SCAT"
 *  @var plane_vars::mrw_gamma
 *  The values of gamma for the Modified Random Walk criterion as described in
 *  \cite Min2009 .
 *  Input label "MRW_GAMMA"
 *
 * ************************************************************************** */

int mu_bins;
int n_photons;
int n_levels;
int output_freq;
int seed;
double tau_max;
double albedo;

/* ************************************************************************** */
/** @struct Photon
 *
 *  @brief Struct used to represent a photon packet in the simulation.
 *
 *  @var Photon::absorb
 *  Set to TRUE if a photon is absorbed.
 *  @var Photon::n_inters
 *  Counter for the number of interactions the photon has undergone.
 *  @var Photon::mrw_steps
 *  Counter for the number of MRW steps the photon has undergone.
 *  @var Photon::mrw_scatter_count
 *  Counter for the number of scatters between MRW transport steps.
 *  @var Photon::x
 *  The x position of the photon.
 *  @var Photon::y
 *  The y position of the photon.
 *  @var Photon::z
 *  The z position of the photon.
 *  @var Photon::cos_theta
 *  The cosine of the photon's theta direction.
 *  @var Photon::sin_theta
 *  The sin of the photon's theta direction.
 *  @var Photon::cos_phi
 *  The cosine of the photon's phi direction.
 *  @var Photon::sin_phi
 *  The sin of the photon's phi direction.
 *
 * ************************************************************************** */

typedef struct Photon
{
    int absorb;
    int n_inters;
    double x;
    double y;
    double z;
    double cos_theta;
    double sin_theta;
    double cos_phi;
    double sin_phi;
} Photon;

/* ************************************************************************** */
/** @struct Mu_hist
 *
 *  @brief Struct used to store the escape angle bins.
 *
 *  @var Mu_hist::bins
 *  Counters for the number of photons escaped from a binned escape angle,
 *  will be an array with mu_bins elements.
 *  @var Mu_hist::theta
 *  The binned escape angles, will be an array with mu_bins elements.
 *
 * ************************************************************************** */

typedef struct Mu_hist
{
    int *bins;
    double *theta;
} Mu_hist;

/* ************************************************************************** */
/** @struct JHK_Moments
 *
 *  @brief Struct used to store the plus and minus moments of the radiation
 *  field.
 *
 *  @var JHK_Moments::j_plus
 *  An array of n_levels elements for the upwards J moment.
 *  @var JHK_Moments::h_plus
 *  An array of n_levels elements for the upwards H moment.
 *  @var JHK_Moments::k_plus
 *  An array of n_levels elements for the upwards K moment.
 *  @var JHK_Moments::j_minus
 *  An array of n_levels elements for the downwards J moment.
 *  @var JHK_Moments::h_minus
 *  An array of n_levels elements for the downwards H moment.
 *  @var JHK_Moments::k_minus
 *  An array of n_levels elements for the downwards K moment.
 *
 * ************************************************************************** */

typedef struct JHK_Moments
{
    double *h_plus;
    double *j_plus;
    double *k_plus;
    double *j_minus;
    double *h_minus;
    double *k_minus;
} JHK_Moments;
