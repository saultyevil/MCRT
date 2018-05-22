import timeit
import numpy as np
from matplotlib import pyplot as plt


def sample_optical_depth(tau_max):
    """
    Sample an optical depth for a photon packet and return the distance the
    photon will move, L = tau/taumax.

    Parameters
    ----------
    tau_max: float.
        The maximum optical depth in the atmosphere.

    Returns
    -------
    tau: float.
        The randomly sampled optical depth.
    L: float.
        The distance the photon packet will travel.
    """

    xi = np.random.rand()
    tau = -np.log(xi)
    L = tau/tau_max

    return tau, L


def emit_photon():
    """
    Emit a photon from the origin of the atmosphere point in a random
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

    # make some random numbers
    xi1 = np.random.random()
    xi2 = np.random.random()

    # compute the sine and cosine of theta and phi, i.e. the directions
    costheta = np.sqrt(xi1)
    sintheta = np.sqrt(1 - costheta ** 2)  # cos ** 2 + sin ** 2 = 1
    phi = 2 * np.pi * xi2
    cosphi = np.cos(phi)
    sinphi = np.sin(phi)
    # set the coords to 0; the origin
    x, y, z = 0, 0, 0

    return x, y, z, costheta, sintheta, cosphi, sinphi


def isotropic_scattering():
    """
    Generate new directions for a photon packet which has been
    isotropically scattered.

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

    # make some random numbers
    xi1 = np.random.random()
    xi2 = np.random.random()

    # compute the scattered angles
    costheta = 2 * xi1 - 1
    sintheta = np.sqrt(1 - costheta ** 2)
    phi = 2 * np.pi * xi2
    cosphi = np.cos(phi)
    sinphi = np.sin(phi)

    return costheta, sintheta, cosphi, sinphi, phi


def bin_photons(mu, mu_bins, n_photons):
    """
    Bins the photons into corresponding angle bins corresponding to the
    direction they left the atmosphere.

    Parameters
    ----------
    mu: (1 x n_photons) array of floats.
        The angle mu = cos(theta) for which the photon packets exited
        the atmospehre.
    mu_bins: integer.
        The number of theta angles to bin for.
    n_photons: integer.
        The total number of photon packets.

    Returns
    -------
    mu_hist: (1 x mu_bins) array of ints.
        The number of photons packets which left the atmosphere at one
        of the binned angles.
    theta: (1 x mu_bins) array of floats.
        The binned theta angles.
    """

    theta = np.zeros(mu_bins)
    mu_hist = np.zeros(mu_bins)

    # calculate the binned theta angles
    dtheta = 1/mu_bins
    half_width = 0.5 * dtheta  # see kenny wood's code for half_width...?
    for i in range(mu_bins):
        theta[i] = np.arccos(i * dtheta + half_width) * (180/np.pi)

    # bin the mu = cos(theta) angles for each photon accordingly
    for i in range(n_photons):
        j = abs(int(mu[i] * mu_bins))
        mu_hist[j] = mu_hist[j] + 1

    return mu_hist, theta


def photon_moments(mu_bins, mu_hist, theta):
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

    Returns
    -------
    intensity: (1 x mu_bins) array of floats.
        The intensity estimated via the number of photon for each binned angle.
    """

    intensity = np.zeros(mu_bins)
    for i in range(mu_bins):
        intensity[i] = (mu_hist[i] * mu_bins)/(2 * np.sum(mu_hist) *
                                               np.cos(theta[i] * np.pi/180))

    return intensity


def photon_transport(n_photons, tau_max, mu_bins, albedo):
    """
    Transport photon packets through a planar atmosphere.

    Parameters
    ----------
    n_photons: integer.
        The number of photon packets to use.
    tau_max: float.
        The maximum optical depth in the atmosphere.
    mu_bins: integer.
        The number of theta angle bins.
    albedo: integer.
        The scattering albedo.
    Returns
    -------
    theta: (1 x mu_bins) array of floats.
        The binned theta angles.
    intensity: (1 x mu_bins) array of floats.
        The intensity for the binned theta angles.
    energy: (1 x mu_bins) array of floats.
        The energy for the binned theta angles.
    """

    start = timeit.default_timer()

    MU = np.zeros(n_photons)
    PHI = np.zeros(n_photons)
    photon_count = 1
    while photon_count <= n_photons:
        # genereate a photon at the origin of the system
        xt, yt, zt, costheta, sintheta, cosphi, sinphi = emit_photon()

        # a photon will scatter at random angles and optical depths until it
        # is either absorbed (terminated) or exits the area
        while (zt >= 0) and (zt <= 1):
            # sample a random optical depth and travel distance
            tau, L = sample_optical_depth(tau_max)
            # update the positions of the photon
            xt += L * sintheta * cosphi
            yt += L * sintheta * sinphi
            zt += L * costheta
            # determine if the photon scatters or not and update direction
            xi = np.random.rand()
            if (xi < albedo) and (zt > 0) and (zt < 1):
                costheta, sintheta, cosphi, sinphi, phi = \
                    isotropic_scattering()

        # if zt < 0, the photon packet has gone into the star hence restart
        # the photon packet. If zt > 1.0, the photon packet has left the
        # star. Record the direction it left the atmosphere
        if (zt > 1.0):
            i = photon_count - 1
            MU[i] = costheta
            PHI[i] = phi
            # if a packet reaches here, we're done with it
            if (photon_count % 5000 == 0):
                print("{} photons completed.".format(photon_count))

            photon_count += 1  # increment photon counter :^)

    mu_hist, theta = bin_photons(MU, mu_bins, n_photons)
    intensity = photon_moments(mu_bins, mu_hist, theta)
    energy = mu_hist/n_photons

    stop = timeit.default_timer()
    print("\nScattering completed in: {:3.2f} seconds.".format(stop - start))

    return theta, intensity, energy


# =============================================================================
# Simulation Parameters & Etc
# =============================================================================

n_photons = int(10e3)
tau_max = 7
mu_bins = 10
albedo = 1

# MCRT simulation
theta, intensity, energy = photon_transport(n_photons, tau_max, mu_bins,
                                            albedo)

# plot the intensity against angle
plt.figure(figsize=(10, 8))
plt.plot(theta, intensity, 'kx')
plt.xlabel(r'Angle, $\theta$')
plt.ylabel(r'Mean Intensity, $J$')
plt.xlim(0, 90)
plt.ylim(0)
plt.show()

# plot the energy absorbed against angle
plt.figure(figsize=(10, 8))
plt.plot(theta, energy, 'kx')
plt.xlabel(r'Angle, $\theta$')
plt.xlim(0, 90)
plt.ylim(0)
plt.show()
