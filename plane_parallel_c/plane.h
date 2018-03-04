/* -------------------------- global variables ------------------------------*/

#define MU_BINS 50   // i hate this
#define MAX_LINE 256

/*-------------------------- struct definitions -----------------------------*/

/* photon packet */
struct photon {
    double x;
    double y;
    double z;
    double costheta;
    double sintheta;
    double cosphi;
    double sinphi;
    double phi;
};

/* histogram for binned angles and count */
struct photon_hist {
    int bins[MU_BINS];
    double theta[MU_BINS];
};

/*---------------------------------------------------------------------------*/


/*-------------- function prototypes of all the functions -------------------*/

/* read in the value of a parameter from file */
double read_in_par(char par[]);
/* generates random numbers */
double random_number(void);
/* prints the current time */
int print_time(void);
/* returns a photon struct which is a photon pointing in a random direction
   at the origin of the slab */
struct photon *emit_photon(void);
/* isotropically scatters a photon packet */
int *scatter_photon(struct photon *packetPtr, double albedo, double tau_max);
/* bins the escape angles of the photon packets and creates a histogram */
struct photon_hist *bin_photons(double mu[], int n_photons, int mu_bins);
/* calculates the moments of the radiation field */
int *calculate_intensity(struct photon_hist *histPtr, double *p, int n_photons, 
int mu_bins);
/* writes out the data to file */
int write_to_file(struct photon_hist *histPtr, double *intensity, int mu_bins);

/*---------------------------------------------------------------------------*/