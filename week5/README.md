Implementation summary

I refactored the string_wave.c code to get rid of coded parameters.

This makes the code more flexible.

Here are the changes I made:

* I added command-line arguments for:

* cycles

* samples

* output file

* I introduced a parameter struct to store settings that are decided at runtime.

* I improved how the code handles input and errors.

I broke the code into parts:

* argument parsing

* initialisation

* update loop

* output generation

* mpi parallelisation

I used MPI to make the program run in parallel.

* I used 1D domain decomposition:

I split the string into parts that are handled by each process.

Each process updates its own part.

* I implemented communication, between neighbouring processes.

I used MPI_Sendrecv to safely send data to neighbours.

I exchanged boundary values at each time step.

Here's how the computation works:

* Each process does:

+ exchange

+ local finite-difference updates

* The code still uses the numerical scheme as the serial version.
