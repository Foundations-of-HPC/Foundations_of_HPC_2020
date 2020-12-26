## Stream benchmark 

In this tutorial we play with the STREAM benchmark to measure memory bandwidth on the HPC multicore nodes.
A few operation will be performed:
  - measure the overall bandwidth on one node and compare with expected peak-performance
  - discuss the role of thread affinity 
  - check the penalties you may pay in case of wrong memory allocation

### compilation 

 We start first compiling the benchmark that is composed by a sigle source code.
 We dowmnload it:

`` wget  ``

and we then compile for both Intel and Gnu compilers using the following set of optimazation flags:


``  make all ``

the benchmark allows to pass some flags to chage the size of the array, that should be at least 4 times the size of L3 caches.
Playing with N in the order of 80 millions should be more than enough on our systems.


### section one: measure bandwidth

#### the naive way 

We can run the code compiled using openMP as it is:

`` 

#### section two: understand the cost of wrong allocation



### References

 - https://www.cs.virginia.edu/stream/ref.html
 - https://community.intel.com/t5/Intel-C-Compiler/STREAM-OMP-benchmark-compiled-with-ICC/td-p/1046864
 - https://blogs.fau.de/hager/archives/8263

