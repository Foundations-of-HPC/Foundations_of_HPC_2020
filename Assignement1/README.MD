
# First Assigment  FHPC course 2020/2021

### Due date : 23.59.02.11.2020
### NEW Due date : 23.59.13.11.2020

***version 1.2 ( released at 19.16 19.10.2020)***

#### Submission policy:  creating a directory on /fast/dssc/Assignement01/username on ORFEO and put all the files required there.

hints:

`` mkdir /fast/dssc/Assignement01/my_username`` 

### COMPUTATIONAL RESOURCES:

I added three more nodes on dssc queue with 24 cores and with a slightly diffferent CPU.
Hyperthreading is not enabled on this nodes.
To select them:




### Structure of directory: automatically checked. 

It should contain the following files:

````
strong-scalability-10to08.csv
strong-scalability-10to09.csv
strong-scalability-10to10.csv
strong-scalability-10to11.csv
weak-scalability-10to08.csv
weak-scalability-10to09.csv
weak-scalability-10to10.csv
weak-scalability-10to11.csv
performance-model.csv
report.pdf 
README
````

where: 

- Report.pdf  will contain a detailed report of the sections  in this documents.

- README should explain which software stack you used to compile the code and run all the program we provided.
 
- all the *.csv file are commma separated values files all structured as follows (automatically checked)

-strong-scalability-10to08 

This is for GPU nodes:
```` 
 #header line: GPU processors,avg,error_bar,run1,run2,run3,... runN
 1,avg,(xmax-xmin)/2,x1,x2,x3,xn.. 
 4......
 8..
 up to 48 in steps of 4 
 where x1,x2,x3,xn..  are the time measured (see later) in different repetitions of the run
```` 
 
This is for THIN nodes:

``` 
 #header line: THIN processors,avg,error_bar,run1,run2,run3,... runN  
 1,avg,(xmax-xmin)/2,x1,x2,x3,xn.. 
 2......
 4..
 up to 24 in steps of 2 
 where x1,x2,x3,xn..  are the time measured (see later) in different repetitions of the run
```` 
 
The file for 100 billion moves (weak-scalability-10to11.csv/ strong-scalability-10to11.csv) should be provided with a less number of steps: 
 - 1/12/24/48 for GPU nodes
 - 1/6/12/24   for THIN nodes 
 

 
 
***We automatically check the structure of the file and the scalability results obtained by us with the same program***  
 	    
## Section  1: theoretical model

- devise a performance model for a simple parallel algorithm: sum of N numbers

  - Serial Algorithm : n-1 operations 

    $T_{serial}= N*T_{comp}$ 
    $T_{comp}$= *time to compute a floating point operation*

  - Parallel Algorithm : master-slave	

    - read N and distribute N to P-1  slaves ===>  $T_{read} + (P-1)\times T_{comm}$ 
      $T_{comm}$ = *time  each processor takes to communicate one message, i.e. latency..*
      $T_{read}$   = *time  master takes to read* 

    - N/P sum over each processors (including master)  ===> $T_{comp}$/N$

    - Slaves send partial sum  ===>   $(P-1)T_{comm}$

    - Master performs  one final sum ===>  $(P-1) \times $T_{comp}$

      the final model:    $T_p=   T_{comp}\times (P -1 + n/P)  + T_{read} + 2(P-1)\times T_{comm}  $

- compute scalability curves for such algorithm and make some plots

  - assumptions:

    - $$T_{comp} =2 \times 10^{-9}$$
    - $T_{read}= 1 \times 10^{-4}$
    - $T_{comm}= 1 \times 10^{-6}$

    Play with some value of N and plot against P (with P ranging from 1 to 100)

- Comment on them in the report:

  - For which values of N do you see the algorithm scaling ? 

  - For which values of P does the algorithm produce the best results ?

  - Can you try to modify the algorithm sketched above to increase its scalability ? 

    (hints: try to think a  better communication  algorithm) 
    
  -Fill in a cvs file with the value of P which gets the best results for the following values of N

provide a csv with the following format

```
#header: N, best P naive algorithm , best P for enhanced algorithm if any, if not just put XXX
i.e

####
20000,  Pn,Pe
100000, Pn,Pe
200000, Pn,Pe
1000000,Pn,Pe
20000000,Pn,Pe

````


## Section 2 : play with MPI program 

### 2.1:  compute strong scalability of a mpi_pi.c program

The application we are using here is a toy application already discussed in a tutorial.
It performs a Monte-Carlo integration to compute PI; the idea is to have a circle inscribed inside a square of unit lenght. The ratio between the area of the square (1) and the circle (pi/4) is $\pi/4$. Therefore, if we randomly choose N points inside the square, on average, only `M=N*pi/4` points will belong to the circle. From the last relation we can estimate pi. 
We provide a basic serial implementation of the algorithm ( program pi.c ) and we also give a parallel MPI implementation ( mpi_pi.c ) that computes PI by the same algorithm using MPI to compute the algorithm in parallel. Your exercise is to see how well this application scales up to the total number of cores of one node. You can modify the codes we have given, or you can write your own. In this case please provide the code and specify this in the README file.


Steps to do:

- Compile the serial and parallel version.
- Determine the CPU time required to calculate PI with the serial calculation using 10000000 (100 millions) iterations (stone throws). Make sure that this is the actual run time and does not include any system time.

Hint: use ``/usr/bin/time`` command to time all the applications and consider here the walltime(elapsed) time as the right one.

- Get the MPI code running for the same number of iterations (i.e. moves) on one processor.

comparison of this to the serial calculation gives you some idea of the overhead associated with MPI. Report and discuss if there are difference in performance betwen the two codes on one single core.  

Please note that the parallel code writes walltime for all the processor involved. Which of these times do you want to consider to estimate the parallel time ?

- let us now do some running that constitutes a strong scaling test.

We will keep the problem size constant, or in other words, keeping Niter = 100 millions. 

- Keeping Niter = 100 millions, run the MPI code for 1, 4, 8 up to 48 MPI processs.

In principle, all that needs to be done is to run the program multiple times, changing the -np argument to mpirun each time. Such a study could be as simple as the following bash for loop:

​```
 for procs in 1 2 4 8 16 24 ... 48 ; do
 time mpirun -np ${procs} my-program the-input >out.${procs}
 done
​```

This script, enhanced as you like and as explained during the last tutorial, have to submitted on the ORFEO by means of PBSPro command qsub.
In any case it is important to script your job executions, so that you can quickly measure scalability. So in this part of the assignement you need to implement a strong scaling script by means of bash scripting, or if you prefer you can use python as well.
Please also repeat the measurements at least three time and report the average values and estimates the error bar as simple as the largest value minus the smallest one. 

- Make a plot of run time  versus number of nodes from the data you have collected.
   - please consider both elapsed time collected by `/usr/bin/time` command and the internal time (take the highest time among all the processors)
   - provide a plot where you compare the scalability curves obtained using these two different timing modality: comment the results.
   - Perfect scalability here would yield a straight line graph. Comment on your results. Repeat the work playing with a large dataset (i.e. enlarge the number of iterations with three other values: 1 billion, 10 billions and 100 billions. 
    
- Provide a table as explained above for each measurement final plot with at least 3 different sizes  and for each of the them please report and comment your final results; to produce this final graph just use one of the two times at your choice. Comment again the results and your choice.  

### 2.2: identify a model for the parallel overhead

In this section you are asked to identify a simple model that estimate the parallel overhead of our simple program as a function of the number of processor. We ask here to extend the simple model discussed previously and adapt/modify to the present program.

To do this please review carefully all the data collected so far and identify which is the indirect method to measure the parallel overhead from the output of the program.


### 2.3: weak scaling 

- Now let us do some running that constitutes a weak scaling test.

This means increasing the problem size simultaneously with the number of nodes being used. In the present case, increasing the number of iterations, Niter.

- Record the run time for each number of nodes and make a plot of the run time versus number of computing nodes.

- Weak scaling would imply that the runtime remains constant as the problem size ( i.e. number of moves, aka iterations)  and the number of compute nodes increase in proportion. Modify your scripts to rapidly collect numbers for the weak scalability tests for differrent number of moves.

- Plot on the same graph the efficiency (T(1)/T(p)) of weak scalability for the different number of iterations and comment the results obtained.
 
Comment: In this last point we are measuring weak scalability and we define above a sort of "weak efficiency"  as the ratio T(1)/T(P) as a way to  be able to plot within a single graph with the same scale the weak scaling behavior for different numbers of moves (i.e. different problem sizes) and make some comparison among them..

