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
        The sine of scattered the phi direction.
    """

    xi1 = np.random.random()
    xi2 = np.random.random()

    costheta = 2 * xi1 - 1
    sintheta = np.sqrt(1 - costheta ** 2)
    phi = 2 * np.pi * xi2
    cosphi = np.cos(phi)
    sinphi = np.sin(phi)

    return costheta, sintheta, cosphi, sinphi, phi

# =============================================================================
# Simulation Parameters
# =============================================================================

start = timeit.default_timer()

# set simulation parameters
NPhotons = int(10e4)
tau_max = 10
albedo = 1

MU = np.zeros(NPhotons)
PHI = np.zeros(NPhotons)

if NPhotons:
    for ph in range(NPhotons):
        # genereate a photon at the origin of the system
        xt, yt, zt, costheta, sintheta, cosphi, sinphi = emit_photon()

        # a photon will scatter at random angles and optical depths until it
        # is either absorbed (terminated) or exits the area
        while (zt >= 0) and (zt <= 1):
            # assign original positions for photons
            x = xt
            y = yt
            z = zt
            # sample a random optical depth and hence travel distance
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
        # gone into the star
        # if zt > 1, the photon has left through the top of the plane, record
        # the direction the photon left
        if (zt >= 1):
            MU[ph] = costheta
            PHI[ph] = phi

    stop = timeit.default_timer()
    print("Scattering completed in: {:3.2f} seconds.".format(stop - start))
else:
    stop = timeit.default_timer()
    print("Scattering  of zero photons completed in: {:3.2f} seconds."
          .format(stop - start))
