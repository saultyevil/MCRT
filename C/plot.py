import numpy as np
from matplotlib import pyplot as plt

data = np.loadtxt('intensity.txt', skiprows=1)

theta = data[:, 0] * 180/np.pi
counts = data[:, 1]
intensity = data[:, 2]

fig = plt.figure(figsize=(10, 8))
ax1 = fig.add_subplot(111)
ax1.plot(theta, intensity, 'k-')
ax1.set_xlabel(r'Angle, $\theta$')
ax1.set_ylabel('Normalised Intensity')
ax1.set_xlim(0, 90)

plt.savefig('intensity.pdf')
plt.close()
