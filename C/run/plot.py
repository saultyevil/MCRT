import sys
import numpy as np
from matplotlib import pyplot as plt


def plot_intens(infile="binned_intens.txt", outfile="intens.pdf"):
    """!
    @brief Plot the intensity as a function of mu = cos(theta).

    @param[in] infile: string
        The input file containing the binned escape angles and thr intensity at
        those binned angles.
    @param[in] outfile: string
        The output file name for the plot.

    @return
    """

    data = np.loadtxt(infile, skiprows=1)
    theta = data[:, 0]
    intensity = data[:, 2]

    fig = plt.figure(figsize=(8, 8))

    ax1 = fig.add_subplot(111)
    ax1.semilogy(np.cos(theta), intensity, "k-")
    ax1.set_xlabel(r"$\mu=\cos(\theta)$")
    ax1.set_ylabel("Intensity, $I$")
    ax1.set_xlim(0, 1)

    plt.savefig(outfile)
    plt.close()

    return


if __name__ == "__main__":
    if len(sys.argv) == 2:
        infile = sys.argv[1]
        print("Input: {}".format(infile))
        plot_intens(infile)
    elif len(sys.argv) == 3:
        infile = sys.argv[1]
        outfile = sys.argv[2]
        print("Input: {}".format(infile))
        print("Output: {}".format(outfile))
        plot_intens(infile, outfile)
    else:
        print("Using default input and output filenames")
        plot_intens()
