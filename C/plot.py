import numpy as np
from matplotlib import pyplot as plt

data = np.loadtxt('intensity.txt', skiprows=11)

theta = data[:, 0]
theta_deg = theta[:] * 180/np.pi
counts = data[:, 1]
intensity = data[:, 2]

fig = plt.figure(figsize=(16, 8))
ax1 = fig.add_subplot(121)
ax1.semilogy(theta_deg, intensity, "k-")
ax1.set_xlabel(r"Angle, $\theta$")
ax1.set_ylabel("Normalised Intensity")
ax1.set_xlim(0, 90)

ax2 = fig.add_subplot(122)
ax2.semilogy(np.cos(theta), intensity, "k-")
ax2.set_xlabel(r"$\mu=\cos(\theta)$")
ax2.set_ylabel("Normalised Intensity")
ax2.set_xlim(0, 1)

plt.savefig("intensity.pdf")
plt.close()
