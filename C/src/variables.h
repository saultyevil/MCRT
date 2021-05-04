/* ************************************************************************** */
/** @file variables.h
 *  @author Edward Parkinson
 *  @date 12 July 2018
 *
 *  @brief Useful variables and data types/structures used throughout the code.
 *
 *  @typedef plane_vars::PhotonPacket_t
 *  Struct used to represent a photon packet in the simulation.
 *  @typedef plane_vars::Histogram_t
 *  Struct used to store the escape angle weight.
 *  @typedef plane_vars::Moments_t
 *  Struct used to store the plus and minus moments of the radiation
 *  field.
 *
 * ************************************************************************** */

/* ************************************************************************** */
/**
 *  @def PI
 *  The value of pi to many decimal places.
 *
 *  @def LINE_LEN
 *  The maximum number of characters a line read in file can be.
 *  @def NO_PARAMETER
 *  A value used to determine if a parameter has been found in a file or not.
 *  @def NO_PARAMETER_STRING
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

#define PI 3.1415926535897932

#define LINE_LEN 256
#define NO_PARAMETER '\0'
#define DEFAULT_INI_FILE "plane.input"
#define OUTPUT_FILE_INTENS "intensity.txt"
#define OUTPUT_FILE_MOMENTS "moments.txt"

/* ************************************************************************** */
/**
 *  Global variables
 *
 *  @var plane_vars::N_PHOTONS
 *  The number of Monte Carlo MCRT iterations. Physically, this is the number
 *  of photons which will be transported.
 *  Input label "N_PHOTONS"
 *  @var plane_vars::n_levels
 *  The number of levels used to calculated the moments of the radiation field.
 *  Input label "N_LEVELS"
 *  @var plane_vars::OUTPUT_FREQUENCY
 *  The progress bar output frequency.
 *  Input label "OUTPUT_FREQ"
 *  @var plane_vars::SEED
 *  The SEED used for random number generation.
 *  Input label "SEED"
 *  @var plane_vars::TAU_MAX
 *  The maximum optical depth a photon can traverse.
 *  Input label "TAU_MAX"
 *  @var plane_vars::SCATTERING_ALBEDO
 *  The scattering SCATTERING_ALBEDO for photon interactions.
 *  Input label "ALBEDO"
 *
 * ************************************************************************** */

int N_PHOTONS;
int OUTPUT_FREQUENCY;
int SEED;
double TAU_MAX;
double SCATTERING_ALBEDO;

/* ************************************************************************** */
/** @struct PhotonPacket_t
 *
 *  @brief Struct used to represent a photon packet in the simulation.
 *
 *  @var PhotonPacket_t::absorb
 *  Set to TRUE if a photon is absorbed.
 *  @var PhotonPacket_t::x
 *  The x position of the photon.
 *  @var PhotonPacket_t::y
 *  The y position of the photon.
 *  @var PhotonPacket_t::z
 *  The z position of the photon.
 *  @var PhotonPacket_t::costheta
 *  The cosine of the photon's theta direction.
 *  @var PhotonPacket_t::sintheta
 *  The sin of the photon's theta direction.
 *  @var PhotonPacket_t::cosphi
 *  The cosine of the photon's phi direction.
 *  @var PhotonPacket_t::sinphi
 *  The sin of the photon's phi direction.
 *
 * ************************************************************************** */

typedef struct photon_packet
{
    int absorb;
    int escaped;
    double x, y, z;
    double costheta;
    double sintheta;
    double cosphi;
    double sinphi;
} PhotonPacket_t;

#define PHOTON_INIT {false, false, 0, 0, 0, 0, 0, 0, 0};

/* ************************************************************************** */
/** @struct Histogram_t
 *
 *  @brief Struct used to store the escape angle weight.
 *
 *  @var Histogram_t::n_bins
 *  The number of bins in the histogram.
 *  @var Histogram_t::weight
 *  Counters for the number of photons escaped from a binned escape angle,
 *  will be an array with n_bins elements.
 *  @var Histogram_t::theta
 *  The binned escape angles, will be an array with n_bins elements.
 *
 * ************************************************************************** */

typedef struct intensity_histogram
{
    int n_bins;
    double *weight;
    double *intensity;
    double *theta;
} Histogram_t;

/* ************************************************************************** */
/** @struct Moments_t
 *
 *  @brief Struct used to store the plus and minus moments of the radiation
 *  field.
 *
 *  @var Moments_t::j_plus
 *  An array of n_levels elements for the upwards J moment.
 *  @var Moments_t::h_plus
 *  An array of n_levels elements for the upwards H moment.
 *  @var Moments_t::k_plus
 *  An array of n_levels elements for the upwards K moment.
 *  @var Moments_t::j_minus
 *  An array of n_levels elements for the downwards J moment.
 *  @var Moments_t::h_minus
 *  An array of n_levels elements for the downwards H moment.
 *  @var Moments_t::k_minus
 *  An array of n_levels elements for the downwards K moment.
 *
 * ************************************************************************** */

typedef struct radiation_moments
{
    int n_levels;
    double *h_plus, *h_minus;
    double *j_plus, *j_minus;
    double *k_plus, *k_minus;
} Moments_t;

/* ************************************************************************** */
/**
 *
 *
 * ************************************************************************** */

union ParameterUnion
{
  int _int;
  double _double;
};

#define TYPE_INT 0
#define TYPE_DOUBLE 1
