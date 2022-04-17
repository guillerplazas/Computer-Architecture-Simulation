# Computer-Architecture-Simulation
Class project dedicated to develop and optimize a particle simulation in C++. The program has been implemented in an aos and soa version.

The program will take the following parameters for its correct execution:
 num_objects: integer number, greater or equal than 0, indicating the number of objects
that will be simulated.
 num_iterations: integer number, greater or equal than 0, indicating the number of iterations (time steps) that will be simulated.
 random_seed: positive integer number to be used as a seed for random distribution
generator functions.
Note: Two simulations with the same parameters but different seed will lead to dierent
scenarios.
 size_enclosure: real positive number indicating the size of the simulation enclosure. The
enclosure is considered to be a perfect cube with a vertex in the coordinates origin and with
side equal to size_enclosure.
Consequently, if the value for size_enclosure is n, the opposite vertices fo the cube will
have coordinates (0, 0, 0) and (n, n, n).
 time_step: real positive number indicating the time increment for each iteration of the
simulation.

Once all input parameters have been processed and initial positions and velocities have been
processed for all simulation elements, the program will write this information to a file named
init_config.txt. with the following format:
 All real values will be printed with tree decimal places in fractional part.
 A header line including:
* Size of the enclosure.
* Time step.
* Number of initial objects.
 One line per object where the position (x, y, z coordinates), velocity (x, y, z components)
and mass must be written. This seven values must be separated by blanks.
 Example for an enclosure of size 1000000.0, with time step 0.100 and 2000 initial objects.

Once all simulation iterations have finished the program shall store final data into a file named final_config.txt, with the same format and contents than the file init_config.txt.

More information is to be found in the PDF.


