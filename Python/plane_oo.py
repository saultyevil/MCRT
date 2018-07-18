import timeit
import numpy as np
from matplotlib import pyplot as plt


class Photon_Packet(object):

    def __init__(self):
        """
        Emits a new photon packet.
        """
        self._coords = np.array([0, 0, 0], dtype=np.float)
        self._costheta = np.sqrt(np.random.rand())
        self._sintheta = np.sqrt(1 - self._costheta ** 2)
        self._cosphi = np.cos(2 * np.pi * np.random.rand())
        self._sinphi = np.sqrt(1 - self._cosphi ** 2)
        self._escaped = False

        return None

    def _scatter_packet(self):
        """
        Isotropic scattering of the photon packet, i.e. change the direction
        it propagates.
        """
        self._costheta = 2 * np.random.rand() - 1
        self._sintheta = np.sqrt(1 - self._costheta ** 2)
        self._cosphi = np.cos(2 * np.pi * np.random.rand())
        self._sinphi = np.sqrt(1 - self._cosphi ** 2)

        return 0

    def _transport_packet(self, tau_max, albedo):
        """
        Transport the photon packets through the atmosphere.
        """
        while self._coords[2] >= 0 and self._coords[2] <= 1:
            # sample a random optical depth and update the position of the
            # packet
            tau = -np.log(np.random.rand())
            L = tau/tau_max
            self._coords[0] += L * self._sintheta * self._cosphi
            self._coords[1] += L * self._sintheta * self._sinphi
            self._coords[2] += L * self._costheta
            # if z < 0, the photon has travelled deeper into the atmosphere
            # and is lost, hence restart the photon
            if self._coords[2] < 0:
                self.__init__()
            # if z > 1, the photon has escaped the atmosphere
            elif self._coords[2] > 1:
                self._escaped = True
            # else it's still in the atmosphere and scatter or absorb it
            else:
                xi = np.random.rand()
                if xi < albedo:
                    self._scatter_packet()
                else:
                    break

        return 0


def init_photon_bins(mu_bins):
    """
    Initialise the arrays used to bin photons

    Parameters
    ----------
    mu_bins: integer.
        The number of theta angles to bin for.

    Returns
    -------
    mu_hist: (1 x mu_bins) array of ints.
        An array of 0's.
    theta: (1 x mu_bins) array of floats.
        The binned theta angles.
    """
    theta = np.zeros(mu_bins)
    mu_hist = np.zeros_like(theta)

    dtheta = 1/mu_bins
    half_width = 0.5 * dtheta  # see kenny wood's code for half_width reason
    for i in range(mu_bins):
        theta[i] = np.arccos(i * dtheta + half_width)

    return mu_hist, theta


def bin_photon(cos_theta, mu_bins, mu_hist):
    """
    Bin a photon into the appropriate theta bin
    """
    i = abs(int(cos_theta * mu_bins))
    mu_hist[i] += 1

    return mu_hist


def photon_intensity(mu_bins, mu_hist, theta, n_photons):
    """
    Calculate the mean intensity for the bin angles.

    Parameters
    ----------
    mu_bins: integer.
        The number of theta angle bins.
    mu_hist: (1 x mu_bins) array of ints.
        The number of photons packets which left the atmosphere at one
        of the binned angles.
    theta: (1 x mu_bins) array of floats.
        The binned theta angles.
    n_photons: int.
        The number of photons used in the simulation.

    Returns
    -------
    intens: (1 x mu_bins) array of floats.
        The intensity estimated via the number of photon for each binned angle.
    """
    intens = np.zeros(mu_bins)
    for i in range(mu_bins):
        intens[i] = (mu_hist[i] * mu_bins)/(2 * n_photons * np.cos(theta[i]))

    return intens


# =============================================================================
# Simulation
# =============================================================================
if __name__ == "__main__":
    print('Beginning Simulation...\n')

    # Simulation Parameters
    n_photons = int(10e6)
    mu_bins = 45
    tau_max = 3
    albedo = 0.5
    output_freq = int(10e5)

    start = timeit.default_timer()
    mu = np.zeros(n_photons)
    phi = np.zeros(n_photons)
    mu_hist, theta = init_photon_bins(mu_bins)

    for packet in range(n_photons):
        photon_packet = Photon_Packet()
        photon_packet._transport_packet(tau_max, albedo)
        if photon_packet._escaped is True:
            mu_hist = bin_photon(photon_packet._costheta, mu_bins, mu_hist)
        if ((packet + 1) % output_freq == 0):
            percent_complete = 100 * (packet + 1)/n_photons
            print('{} photons ({:3.1f}%) transported.'
                  .format(packet + 1, percent_complete))

    intensity = photon_intensity(mu_bins, mu_hist, theta, n_photons)
    stop = timeit.default_timer()

    print('\nTransport of {} packets completed in {:3.2f} seconds.'
          .format(n_photons, stop - start))

# =============================================================================
# Plots
# =============================================================================

    fig = plt.figure(figsize=(17, 8))

    # plot the intensity against angle
    ax1 = fig.add_subplot(121)
    ax1.plot(theta, intensity, 'kx')
    ax1.set_xlabel(r'$cos(\theta)$')
    ax1.set_ylabel(r'Normalised Intensity')
    ax1.set_xlim(0, 0.5 * np.pi)

    plt.savefig('mcrt_plane.pdf')
    plt.show()
