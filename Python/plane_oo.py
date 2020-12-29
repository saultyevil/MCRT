"""
A simple script to transport a generation of photons through a 1D homogenous
infinite slab of material. Output is a plot of the photon intensity against
the escape angle from the slab.
"""

import timeit
import numpy as np
from matplotlib import pyplot as plt
from numba import jit, jitclass
from numba import float32, boolean


spec = [
    ("coords", float32[:]),
    ("costheta", float32),
    ("sintheta", float32),
    ("cosphi", float32),
    ("sinphi", float32),
    ("escaped", boolean)
]


@jitclass(spec)
class PhotonPacket(object):
    """Photon packet object. Contains all the functions to create and scatter
    photons."""

    def __init__(self):
        """
        Creates a new photon with a random isotropic direction.
        """

        self.coords = np.zeros(3, np.float32)
        self.costheta = np.sqrt(np.random.rand())
        self.sintheta = np.sqrt(1 - self.costheta ** 2)
        self.cosphi = np.cos(2 * np.pi * np.random.rand())
        self.sinphi = np.sqrt(1 - self.cosphi ** 2)
        self.escaped = False

    def isotropic_scatter(self):
        """
        Scatter a photon isotropically.
        """

        self.costheta = 2 * np.random.rand() - 1
        self.sintheta = np.sqrt(1 - self.costheta ** 2)
        self.cosphi = np.cos(2 * np.pi * np.random.rand())
        self.sinphi = np.sqrt(1 - self.cosphi ** 2)


@jit
def transport_photon_packet(photon, tau_max, albedo):
    """
    Transport a photon packet through the atmosphere.

    Parameters
    ----------
    photon: PhotonPacket
        The photon to transport.
    tau_max: float
        The transverse optical depth of the atmosphere.
    albedo: float
        The scattering albedo in the atmosphere.
    """

    while photon.coords[2] >= 0 and photon.coords[2] <= 1:

        # sample a random optical depth and update the position of the
        # packet

        L = -np.log(np.random.rand()) / tau_max
        photon.coords[0] += L * photon.sintheta * photon.cosphi
        photon.coords[1] += L * photon.sintheta * photon.sinphi
        photon.coords[2] += L * photon.costheta

        # if z < 0, the photon has traveled deeper into the atmosphere
        # and is lost, hence restart the photon
        # if z > 1, the photon has escaped the atmosphere
        # else it's still in the atmosphere and scatter or absorb it

        if photon.coords[2] < 0:
            photon.__init__()
        elif photon.coords[2] > 1:
            photon.escaped = True
        else:
            xi = np.random.rand()
            if xi < albedo:
                photon.isotropic_scatter()
            else:
                break

    return


def init_hist(n_bins):
    """
    Initialize the histogram for binning photon escapes.

    Parameters
    ----------
    n_bins: int.
        The number of bins in the histogram.

    Returns
    -------
    bins: np.ndarray
        The empty angle bins.
    theta: np.ndarray
        The angles of the histogram bins.
    """

    theta = np.zeros(n_bins)
    bins = np.zeros_like(theta)

    dtheta = 1 / n_bins
    half_width = 0.5 * dtheta
    # todo vectorize
    for i in range(n_bins):
        theta[i] = np.arccos(i * dtheta + half_width)

    return bins, theta


def bin_photon(cos_theta, n_bins, hist):
    """
    Bin a photon into the appropriate histogram bin. This histogram basically
    keeps track of the number of photons escaping from the binned angle.

    Parameters
    ----------
    cos_theta: float
        The escape angle of the photon.
    n_bins: int
        The number of bins in the histogram.
    hist: np.ndarray
        The histogram to update.

    Returns
    -------
    hist: np.ndarray
        The updated histogram.
    """

    i = abs(int(cos_theta * n_bins))
    hist[i] += 1

    return hist


def calculate_photon_intensity(n_bins, hist, theta_bins, n_photons):
    """
    Calculate the mean intensity of the binned escape angles.

    Parameters
    ----------
    n_bins: int
        The number of theta angle bins.
    hist: np.ndarray
        The histogram of binned photon numbers.
    theta_bins: np.ndarray
        The binned theta angles.
    n_photons: int.
        The number of photons in the simulation.

    Returns
    -------
    intensity: np.ndarray
        The intensity for each binned angle.
    """

    return hist * n_bins / (2 * n_photons * np.cos(theta_bins))


def main():
    """Main function of the script"""

    n_photons = int(1e6)
    n_bins = 20
    tau_max = 7
    albedo = 1.0
    output_freq = n_photons // 10

    print('Beginning Simulation...\n')

    start = timeit.default_timer()
    hist, theta_bins = init_hist(n_bins)

    for i in range(n_photons):
        pp = PhotonPacket()
        transport_photon_packet(pp, tau_max, albedo)

        if pp.escaped:
            hist = bin_photon(pp.costheta, n_bins, hist)

        if ((i + 1) % output_freq == 0):
            percent_complete = 100 * (i + 1) / n_photons
            print('{} photons ({:3.1f}%) transported.'.format(i + 1, percent_complete))

    intensity = calculate_photon_intensity(n_bins, hist, theta_bins, n_photons)
    stop = timeit.default_timer()

    print('\nTransport of {} packets completed in {:3.2f} seconds.'.format(n_photons, stop - start))

    return theta_bins, intensity


# =============================================================================
# Simulation
# =============================================================================

if __name__ == "__main__":
    theta, intensity = main()
    fig, ax = plt.subplots(figsize=(12, 5))
    ax.plot(theta, intensity, 'kx')
    ax.set_xlabel(r'$cos(\theta)$')
    ax.set_ylabel(r'Normalised Intensity')
    ax.set_xlim(0, 0.5 * np.pi)
    plt.savefig('mcrt_plane.pdf')
    plt.show()
