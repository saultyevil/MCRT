/* **************************************************************************
 *
 * Useful variables and data types are in this header file :-)
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
#define DEFAULT_INI_FILE "plane.ini"
#define OUTPUT_FILE_INTENS "intensity.txt"
#define OUTPUT_FILE_MOMENTS "moments.txt"

/*
 * Global variables which are used practically everywhere in the program. The
 * values of these variables are read in from a provided ini file.
 *
 *  - mu_bins: the number of angles to be binned.
 *  - n_photons: the number of photons in the simulation.
 *  - tau_max: the maximum optical depth a photon can experience.
 *  - albedo: the reflection parameter, i.e. albedo = 1 results in scattering at
 *    each interaction, whereas albedo = 0 will result in absorption events.
 */
int mu_bins;
int n_photons;
int n_levels;
int output_freq;
double tau_max;
double albedo;

/*
 * Photon packet data type - used for storing all the data related to a photon
 * packet.
 *
 *  Parameters:
 *  - absorb: flag for if a phonton was absorbed
 *  - n_inters: count for how many interactions undergone
 *  - x: x-coord of the photon
 *  - y: y-coord of the photon
 *  - z: z-coord of the phone
 *  - cos_theta: the cosine of the theta angular dependence
 *  - sin_theta: the sine of the theta angular dependence
 *  - cos_phi: the cosine of the phi angular dependence
 *  - sin_phi: the sine of the phi angular dependence
 */
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

/* Histogram data type - used for storing the binned angles and the binned angle
 * counts. Both parameters in the data type are pointers, which will ideally
 * point to an array of length mu_bins.
 *
 * Parameters:
 *  - *bins: the binned angle counts.
 *  - *theta: the binned angles.
 */
typedef struct Photon_hist
{
    int *bins;
    double *theta;
} Photon_hist;

/*
 * Moments data type - used for storing the moments of the radiation field.
 */
typedef struct Moments
{
    double *j_plus;
    double *h_plus;
    double *k_plus;
    double *j_minus;
    double *h_minus;
    double *k_minus;
} Moments;
