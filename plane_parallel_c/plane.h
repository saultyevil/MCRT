/* -------------------------- global variables ------------------------------*/

#define MU_BINS 30   /* i hate this */
#define MAX_LINE 256 /* max length an input par line can be */
#define TRUE 1  /* define the value of TRUE */

/*-------------------------- struct definitions -----------------------------*/

/* photon packet */
struct photon
{
    double x;  /* x coordinate of packet */
    double y;  /* y coordinate of packet */
    double z;  /* z coordinate of packet */
    double costheta;  /* sampled mu = cos(theta) direction */
    double sintheta;
    double phi; /* sampled phi direction */
    double cosphi;
    double sinphi;
    int interactions;  /* counter for the number of interactions undergone */
    double avg_tau;  /* records the average optical depth moved thru */
    double avg_L;   /* records the average dispalcement moved */
};

/* histogram for binned angles and count */
struct photon_hist
{
    double theta[MU_BINS];  /* the binned escape angles */
    int bins[MU_BINS];  /* the packet count at each escape angle */
};

/*-------------- function prototypes of all the functions -------------------*/

/* generates random numbers */
double random_number(void);
/* prints the current time */
int print_time(void);
/* creates a photon packet */
struct photon *emit_photon(void);
/* transport a photon packet through atmosphere with istroptic scattering */
void transport_photon(struct photon *packetPtr, double albedo, double tau_max);
/* bins the escape angles of the photon packets and creates a histogram */
struct photon_hist *bin_photons(double mu[], int n_photons, int mu_bins);
/* calculates the normalised intensity of the radiation field */
double *calculate_intensity(struct photon_hist *histPtr, int n_photons,
int mu_bins);
/* read in the value of a parameter from file */
double read_in_par(char par[]);
/* writes out the data to file */
int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins);

/*---------------------------------------------------------------------------*/
