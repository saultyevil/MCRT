import sys
import numpy as np
from matplotlib import pyplot as plt


def plot_intens(type="mu", infile="binned_intens.txt", outfile="intens.pdf"):
    """!
    @brief Plot the intensity as a function of mu = cos(theta).

    @param[in] type: string
        The type of graph to create,
            * mu -> x axis is mu = cos(theta)
            * deg -> x axis is theta in degrees
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

    if type == "mu":
        ax1.semilogy(np.cos(theta), intensity, "k-")
        ax1.set_xlabel(r"$\mu=\cos(\theta)$")
        ax1.set_ylabel("Intensity, $I$")
        ax1.set_xlim(0, 1)
    elif type == "deg":
        ax1.semilogy(np.degrees(theta), intensity, "k-")
        ax1.set_xlabel(r"$\theta$ (degrees)")
        ax1.set_ylabel("Intensity, $I$")
        ax1.set_xlim(0, 90)
    else:
        print("\n--------------------------------------------------")
        print(" Didn't understand type input, try 'mu' or 'deg'.")
        print("--------------------------------------------------\n")


    plt.savefig(outfile)
    plt.close()

    return


if __name__ == "__main__":
    if len(sys.argv) == 2:
        type = sys.argv[1]
        plot_intens(type)
    elif len(sys.argv) == 3:
        type = sys.argv[1]
        infile = sys.argv[2]
        plot_intens(type, infile)
    elif len(sys.argv) == 4:
        type = sys.argv[1]
        infile = sys.argv[2]
        outfile = sys.argv[3]
        plot_intens(type, infile, outfile)
    else:
        plot_intens()
