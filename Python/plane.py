import timeit
import numpy as np
from numba import jit
from matplotlib import pyplot as plt


def emit_photon():
    """
    """

    # generate random numbers
    xi1, xi2 = np.random.rand(2)
    # compute the sine and cosine of theta and phi, i.e. the directions
    costheta = np.sqrt(xi1)
    sintheta = np.sqrt(1 - costheta ** 2)  # cos ** 2 + sin ** 2 = 1
    cosphi = np.cos(2 * np.pi * xi2)
    sinphi = np.sqrt(1 - cosphi ** 2)
    # set the coords to 0; the origin
    pos = np.array([0.0, 0.0, 0.0])

    return pos, np.array([costheta, sintheta, cosphi, sinphi])


def init_photon_bins(mu_bins):
    """
    """

    mu_hist = np.zeros(mu_bins)
    theta = np.zeros(mu_bins)
    # calculate the escape angles which will be binned and populate the array
    d_theta = 1/mu_bins
    half_width = 0.5 * d_theta
    for i in range(mu_bins):
        theta[i] = np.arccos(i * d_theta + half_width)

    return mu_hist, theta


def bin_photon(mu_hist, cos_theta, mu_bins):
    """
    """

    i = abs(int(cos_theta * mu_bins))
    mu_hist[i] += 1

    return mu_hist


def sample_distance(tau_max):
    """
    """

    tau = -np.log(np.random.rand())
    L = tau/tau_max

    return L


def isotropic_scattering():
    """
    """

    # generate random numbers
    xi1, xi2 = np.random.rand(2)
    # compute the scattered direction
    costheta = 2 * xi1 - 1
    sintheta = np.sqrt(1 - costheta ** 2)
    cosphi = np.cos(2 * np.pi * xi2)
    sinphi = np.sqrt(1 - cosphi ** 2)

    return np.array([costheta, sintheta, cosphi, sinphi])


@jit
def transport_photons(n_photons, tau_max, mu_bins, albedo, output_freq):
    """
    """

    mu_hist, theta = init_photon_bins(mu_bins)
    for photon_count in range(n_photons):
        coord, direction = emit_photon()
        while coord[2] >= 0 and coord[2] <= 1:
            # sample random distance and update the position of the photon
            L = sample_distance(tau_max)
            coord[0] += L * direction[1] * direction[2]
            coord[1] += L * direction[1] * direction[3]
            coord[2] += L * direction[0]
            if coord[2] < 0:
                coord, direction = emit_photon()
            elif coord[2] > 1:
                mu_hist = bin_photon(mu_hist, direction[0], mu_bins)
                break
            else:
                if np.random.rand() < albedo:  # isotropic scattering
                    direction = isotropic_scattering()
                else:  # packet is absorbed
                    break

        if ((photon_count + 1) % output_freq == 0):
            print("{} photons completed ({:3.1f}%).".format(photon_count+1,
                  (photon_count+1)/n_photons * 100))

    return mu_hist, theta


def calculate_intensity(mu_hist, theta, mu_bins, n_photons):
    """
    """

    intens = np.zeros(mu_bins)
    for i in range(mu_bins):
        intens[i] = (mu_hist[i] * mu_bins)/(2 * n_photons * np.cos(theta[i]))

    return intens


def plot_intensity(theta, intensity):
    """
    """

    plt.figure(figsize=(8, 8))
    plt.semilogy(np.cos(theta), intensity, 'k-')
    plt.xlabel(r"$\mu=\cos(\theta)$")
    plt.ylabel("Normamlised Intensity")
    plt.xlim(0, 1)
    plt.savefig("intensity.pdf")
    plt.show()

    return 0


def start_mcrt(n_photons, tau_max, mu_bins, albedo, output_freq):
    """
    """

    print("Beginning MCRT simulation...\n")
    start = timeit.default_timer()
    mu_hist, theta = transport_photons(n_photons, tau_max, mu_bins, albedo,
                                       output_freq)
    intensity = calculate_intensity(mu_hist, theta, mu_bins, n_photons)
    stop = timeit.default_timer()
    plot_intensity(theta, intensity)
    print("\nScattering completed in: {:3.2f} seconds.".format(stop - start))

    return theta, intensity


# =============================================================================
# Simulation Parameters
# =============================================================================

if __name__ == "__main__":
    # Run the simulation
    n_photons = 1000000
    output_freq = 100000
    mu_bins = 41
    tau_max = 5
    albedo = 0.5
    theta, intensity = start_mcrt(n_photons, tau_max, mu_bins, albedo,
                                  output_freq)
