/* ------------------------- struct definitions ----------------------------- */

#define MU_BINS 30   // i hate this

/*
 * Photon packet struct
 */
struct photon
{
    double x;           // x coordinate of packet
    double y;           // y coordinate of packet
    double z;           // z coordinate of packet
    double costheta;    // sampled mu = cos(theta) directions
    double sintheta;
    double phi;         // sampled phi directions
    double cosphi;
    double sinphi;
    int interactions;   // counter for the number of interactions undergone
    double avg_tau;     // records the average optical depth moved thru
    double avg_L;       // records the average dispalcement moved
};

/*
 * Histogram for binned angles and count
 */
struct photon_hist
{
    double theta[MU_BINS];  // the binned escape angles
    int bins[MU_BINS];      // the packet count at each escape angle
};

/* ------------------------ Utility functions ------------------------------- */

#define TRUE 1
#define FALSE 0

/*
 * Generates a random number
 */
double random_number(void);
/*
 * Prints the current time
 */
int print_time(void);

/* -------------------- Input and output functions -------------------------- */

#define MAX_LINE 128
#define INI_FILE "plane.ini"
#define OUTPUT_FILE "intensity.txt"
#define NO_PAR_CONST -1
#define VERBOSE_OUTPUT_FILE "verbose_logging.txt"

/*
 * Read in the a double of a parameter from file
 */
int read_double(char *par_string, double *parameter);
/*
 * Read in the a long of a parameter from file
 */
int read_long(char *par_string, long *parameter);
/*
 * Writes simulation output to file
 */
int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins);

/* ---------------------- MC transport functions ---------------------------- */

/*
 * Create a photon packet
 */
int emit_photon(struct photon *packetPtr);
/*
 * Transport a photon packet with istroptic scattering
 */
int transport_photon(struct photon *packetPtr, double albedo, double tau_max);
/*
 * Bins the escape angles of the photon packets and creates a histogram
 */
int bin_photons(struct photon_hist *histPtr, double *mu, int n_photons,
    int mu_bins);
/*
 * Calculates the normalised intensity of the radiation field
 */
int calculate_intensity(double *intensity, struct photon_hist *hist,
    int n_photons, int mu_bins);
