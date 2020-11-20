**Assignement 1**

**Section 1:**
Considering the final formula for the parallel calculation required, different time values are obtained as a function of the number of processors (as shown in the report). These values are found in the performance-model.csv file.
**Section2:**

I run the serial tests using the script `serial.script`, that has the command: `/usr/bin/time ./pi.x ${MOVES} >out.serial 2>> serialTIMES.dat `

From the`mpi_pi.c` and with some modifications I get the executable `mpi_pi.x` 

I use the module `openmpi/4.0.3/gnu/9.3.0`, and I run the mpi tests  with a command inside as: `/usr/bin/time -f %e mpirun --mca btl '^openib' -np ${procs} mpi_pi.x ${MOVES} >strongout.${procs} 2>>strongTIMES.dat`

For the weak scalability I use:

``/usr/bin/time -f %e mpirun --mca btl '^openib' -np ${procs} mpi_pi.x $(${procs}*${MOVES}) >weakout.${procs} 2>> weakTIMES.dat`





