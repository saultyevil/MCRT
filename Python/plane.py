"""
A simple script to transport a generation of photons through a 1D homogenous
infinite slab of material. Output is a plot of the photon intensity against
the escape angle from the slab.
"""

import timeit
import numpy as np
from numba import jit
from matplotlib import pyplot as plt


def emit_photon():
    """
    Emit a photon from the origin of the slab with a random isotropic direction.

    Returns
    -------
    pos: np.ndarray
        The 3d position of the photon at the origin.
    direction: np.ndarray
        The cos and sin of the theta and phi direction of the photon.
    """

    # generate random numbers
    xi1, xi2 = np.random.rand(2)
    # compute the sine and cosine of theta and phi, i.e. the directions
    costheta = np.sqrt(xi1)
    sintheta = np.sqrt(1 - costheta ** 2)  # cos ** 2 + sin ** 2 = 1
    cosphi = np.cos(2 * np.pi * xi2)
    sinphi = np.sqrt(1 - cosphi ** 2)
    # set the coords to the origin
    pos = np.array([0.0, 0.0, 0.0])

    return pos, np.array([costheta, sintheta, cosphi, sinphi])


def init_hist(n_bins):
    """
    Initialize the histogram to bin escaping photons.

    Parameters
    ----------
    n_bins: int
        The number of bins in the histogram.

    Returns
    -------
    hist: np.ndarray
        An empty array of n_bins elements.
    theta: np.ndarray
        The angle values for the histogram bins, from 0 to pi/2 radians.
    """

    hist = np.zeros(n_bins)
    theta = np.zeros_like(hist)

    # calculate the escape angles which will be binned and populate the array

    d_theta = 1 / n_bins
    half_width = 0.5 * d_theta
    for i in range(n_bins):
        theta[i] = np.arccos(i * d_theta + half_width)

    return hist, theta


def bin_photon(hist, cos_theta, n_bins):
    """
    Bin a photon's escape direction into the histogram.

    Parameters
    ----------
    hist: np.ndarray
        The histogram to bin.
    cos_theta: float
        The cosine of the escape direction of the photon.
    n_bins: int
        The number of bins in the histogram.

    Returns
    -------
    hist: np.ndarray
        The updated histogram.
    """

    hist[abs(int(cos_theta * n_bins))] += 1

    return hist


def isotropic_direction():
    """
    Sample a new isotropic direction.
    """

    xi1, xi2 = np.random.rand(2)
    costheta = 2 * xi1 - 1
    sintheta = np.sqrt(1 - costheta ** 2)
    cosphi = np.cos(2 * np.pi * xi2)
    sinphi = np.sqrt(1 - cosphi ** 2)

    return np.array([costheta, sintheta, cosphi, sinphi])


@jit
def transport_photons(n_photons, tau_max, n_bins, albedo, output_freq):
    """
    Transport a generation of photons through the atmosphere. The output of this
    function is a histogram of the number of photons which escaped with the
    direction of each angle bin in the histogram. The histogram bins are
    mu = cos(theta).

    Parameters
    ----------
    n_photons: int
        The number of photons to transport.
    tau_max: float
        The transverse optical depth of the slab.
    n_bins: int
        The number of bins in the output histogram of escape angle.
    albedo: float
        The scattering albedo in the atmosphere.
    output_freq: int
        The frequency a progress update should be output. This is just a number.

    Returns
    -------
    hist: np.ndarray
        The histogram of binned photon counts.
    theta: np.ndarray
        The escape angles for the histogram bins.
    """

    hist, theta = init_hist(n_bins)
    for i in range(n_photons):
        coord, direction = emit_photon()

        while coord[2] >= 0 and coord[2] <= 1:
            # sample random distance and update the position of the photon
            length = -np.log(np.random.rand()) / tau_max
            coord[0] += length * direction[1] * direction[2]
            coord[1] += length * direction[1] * direction[3]
            coord[2] += length * direction[0]

            if coord[2] < 0:    # Hit the bottom, re-emit
                coord, direction = emit_photon()
            elif coord[2] > 1:  # Escaped
                hist = bin_photon(hist, direction[0], n_bins)
                break
            else:               # Scattered
                if np.random.rand() < albedo:
                    direction = isotropic_direction()
                else:            # Packet is absorbed
                    break

        if (i + 1) % output_freq == 0:
            print("{} photons completed ({:3.1f}%).".format(i + 1, (i + 1) / n_photons * 100))

    return hist, theta


def calculate_intensity(hist, theta, n_bins, n_photons):
    """
    Convert the photon counts into an intensity.

    Parameters
    ----------
    hist: np.ndarray
        The histogram of photon counts.
    theta: np.ndarray
        The angle bins of the histogram.
    n_bins: int
        The number of bins in the histogram.
    n_photons: int
        The number of photons transported.

    Returns
    -------
    intensity: np.ndarray
        The intensity for each angle bin.
    """

    return hist * n_bins / (2 * n_photons * np.cos(theta))


def plot_intensity(theta, intensity):
    """
    Plot the normalized intensity against angle.

    Parameters
    ----------
    theta: np.ndarray
        The angle bins of the intensity histogram.
    intensity: np.ndarray
        The intensity of each angle bin.
    """

    fig, ax = plt.subplots(figsize=(8, 8))
    ax.semilogy(np.cos(theta), intensity, 'k-')
    ax.set_xlabel(r"$\mu=\cos(\theta)$")
    ax.set_ylabel("Normamlised Intensity")
    ax.set_xlim(0, 1)
    plt.show()

    return


def main():
    """Main function of the script"""

    n_photons = int(1e5)
    output_freq = int(1e4)
    n_bins = 20
    tau_max = 7
    albedo = 1.0

    print("Beginning MCRT simulation...\n")

    start = timeit.default_timer()

    hist, theta = transport_photons(n_photons, tau_max, n_bins, albedo, output_freq)
    intensity = calculate_intensity(hist, theta, n_bins, n_photons)

    print("\nSimulation completed in: {:3.2f} seconds.".format(timeit.default_timer() - start))

    plot_intensity(theta, intensity)

    return theta, intensity


# =============================================================================
# Simulation Parameters
# =============================================================================

if __name__ == "__main__":
    t, i = main()
