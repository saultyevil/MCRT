Monte Carlo Radiative Transfer codes. 

# plane_parallel_py
## plane.py
   Standard Python approach. Simulates the transport of a photon packet through
   a star's outer atmosphere. 
## plane_oo.py
   Object oriented approach. Simulates the transport of a photon packet through
   a star's outer atmosphere.
## plane_parallel.ipynb
   Jupyter notebook version of plane_oo.py.

# plane_parallel.c
   Contains a C code to simulate the transport of a photon packet through a 
   star's outer atmosphere. Reads in parameters from a file and outputs the
   data to a file. plot.py plots the output into a pdf. To compile, type
   make whilst in a terminal in the directory. Run by typing ./plane. The 
   parameters can be changed in plane.ini.
