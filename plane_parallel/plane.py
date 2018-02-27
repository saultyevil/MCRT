# =============================================================================
# Radiative Transfer of Photons: plane parallel atmosphere.
# Photons will be isotropically scattered, so every intereaction of a photon
# will be a scattering event.
# =============================================================================

import numpy as np
import timeit
from matplotlib import pyplot as plt


def sample_optical_depth(tau_max):
    """
    Randomly samples an optical depth and distance travelled for a photon.

    Parameters
    ----------
    tau_max: float.
        The maximum optical depth in the parallel plane.

    Returns
    -------
    tau: float.
        The sampled value of the optical depth.
    L: float.
        The distance the photon will travel due to the optical depth sampled.
    """

    xi = np.random.random()
    tau = -np.log(xi)
    L = tau/tau_max

    return tau, L


def emit_photon():
    """
    Generates a photon from the origin of a system (0, 0, 0) with a random
    direction.

    Returns
    -------
    x: float.
        The x coordinate of the photon.
    y: float.
        The y coordinate of the photon.
    z: float.
        The z coorindate of the photon.
    costheta: float.
        The cosine of the theta direction.
    sintheta: float.
        The sine of the theta direction.
    cosphi: float.
        The cosine of the phi direction.
    sinphi: float.
        The sine of the phi direction.
    """

    xi1 = np.random.random()
    xi2 = np.random.random()
    # generate a photon's angles using random sampling
    costheta = np.sqrt(xi1)
    sintheta = np.sqrt(1 - costheta ** 2)  # cos ** 2 + sin ** 2 = 1
    phi = 2 * np.pi * xi2
    cosphi = np.cos(phi)
    sinphi = np.sin(phi)
    # set the photon to be at the origin of a cartersian grid
    x, y, z = 0, 0, 0

    return x, y, z, costheta, sintheta, cosphi, sinphi


def isotropic_scattering():
    """
    Generates a new direction for a photon which has been isotropically
    scattered.

    Returns
    -------
    costheta: float.
        The cosine of the scattered theta direction.
    sintheta: float.
        The sine of the scattered theta direction.
    cosphi: float.
        The cosine of the scattered phi direction.
    sinphi: float.
        The sine of the scattered the phi direction.
    """

    xi1 = np.random.random()
    xi2 = np.random.random()

    costheta = 2 * xi1 - 1
    sintheta = np.sqrt(1 - costheta ** 2)
    phi = 2 * np.pi * xi2
    cosphi = np.cos(phi)
    sinphi = np.sin(phi)

    return costheta, sintheta, cosphi, sinphi, phi


def bin_photons(mu, mu_bins, n_photons):
    """
    Bins the photons into corresponding angle bins corresponding to the
    direction they left the slab.

    Parameters
    ----------
    mu: (1 x n_photons) array of floats.
        The angle mu = cos(theta) which the photons left the slab.
    mu_bins: integer.
        The number of angle bins to bin for. The angle bins are from
        0 < mu < 1, hence 0 < theta < 90
    n_photons: integer.
        The number of photons in the MC simulation.

    Returns
    -------
    mu_hist: (1 x mu_bins) array of ints.
        An array containing the number of photons in the binned angles, i.e.
        the number of photon packets which leave the plane in the binned angle
        range.
    theta: (1 x mu_bins) array of floats.
        An array containing the theta angles for which the photons have been
        binned.
    """

    dtheta = 1/mu_bins
    half_width = 0.5 * dtheta  # see kenny wood's code for half_width...?

    theta = np.zeros(mu_bins)
    mu_hist = np.zeros(mu_bins)

    # calculate the theta binning angles
    for i in range(mu_bins):
        theta[i] = np.arccos(i * dtheta + half_width) * (180/np.pi)

    # bin the mu = cos(theta) angles accordingly
    for i in range(n_photons):
        j = abs(int(mu[i] * mu_bins))
        mu_hist[j] = mu_hist[j] + 1

    return mu_hist, theta


def moments(mu_bins, mu_hist, theta):
    """
    Calculate the intensity of the radiation field by counting photons.

    Parameters
    ----------
    mu_bins: integer.
        The number of bins in the range 0 - 90 degrees.
    mu_hist: (1 x mu_bins) array of ints.
        An array containing the number of photons in the binned angles, i.e.
        the number of photon packets which leave the plane in the binned angle
    theta: (1 x mu_bins) array of floats.
        An array containing the theta angles for which the photons have been
        binned.

    Returns
    -------
    intensity: (1 x mu_bins) array of floats.
        The intensity estimated via the number of photon in each binned angle.
    """

    intensity = np.zeros(mu_bins)

    for i in range(mu_bins):
        intensity[i] = (mu_hist[i] * mu_bins)/(2 * np.sum(mu_hist) *
                                               np.cos(theta[i] * np.pi/180))

    return intensity


# =============================================================================
# Simulation Parameters
# =============================================================================

start = timeit.default_timer()

# set simulation parameters, 10e4 photon packets is ~5 minutes
n_photons = int(10e4)
tau_max = 17
mu_bins = 20
albedo = 1

MU = np.zeros(n_photons)
PHI = np.zeros(n_photons)

if n_photons:
    photon_count = 1
    while photon_count <= n_photons:
        # genereate a photon at the origin of the system
        xt, yt, zt, costheta, sintheta, cosphi, sinphi = emit_photon()

        # a photon will scatter at random angles and optical depths until it
        # is either absorbed (terminated) or exits the area
        while (zt >= 0) and (zt <= 1):
            # assign original positions for photons
            x = xt
            y = yt
            z = zt
            # sample a random optical depth and travel distance
            tau, L = sample_optical_depth(tau_max)
            # update the positions of the photon
            xt = x + L * sintheta * cosphi
            yt = y + L * sintheta * sinphi
            zt = z + L * costheta

            # determine if the photon scatters or not and update direction
            xi = np.random.rand()
            if (xi < albedo) and (zt > 0) and (zt < 1):
                costheta, sintheta, cosphi, sinphi, phi = \
                    isotropic_scattering()

        # if zt < 0, the photon has left through the bottom of the plane, and
        # gone into the star therefore restart the packet
        # if zt > 1, the photon has left through the top of the plane, record
        # the direction the photon left
        if (zt >= 1.0):
            i = photon_count - 1  # python uses zero indexing so etc
            MU[i] = costheta
            PHI[i] = phi

            photon_count += 1  # increment photon counter :^)

    mu_hist, theta = bin_photons(MU, mu_bins, n_photons)
    intensity = moments(mu_bins, mu_hist, theta)

    # plot the intensity against angle
    plt.figure(figsize=(8, 8))
    plt.plot(theta, intensity, 'o')
    plt.xlabel(r'Angle, $\theta$')
    plt.ylabel(r'Mean Intensity, $J$')
    plt.savefig('intensity.pdf')
    plt.show()

    stop = timeit.default_timer()
    print("Scattering completed in: {:3.2f} seconds.".format(stop - start))
else:
    stop = timeit.default_timer()
    print("Scattering  of zero photons completed in: {:3.2f} seconds."
          .format(stop - start))
