## Day 19:  Again on Performance Model and  a short tutorial on measuring MPI performance on ORFEO nodes 

date: Wednesday 9/12/2020

### lectures
 - Stefano Cozzini : [Network Hardware and Performance Model to evaluate MPI applications](lecture8-PerformanceEvaluationforMPI-application.pdf)


### references
 - chapter 9 of reference 4: final section: code here provided is taken from the [book's website](https://blogs.fau.de/hager/hpc-book#teaching) 
 - chapter 10 of reference 4: final section contains interesting stuff. 
 
### tips/notes on the provided code 

It is a "simple" fortran implementation of the Jacobi solver in 3D 

To compile, load the MPI module and then: 

 ` mpif77 -ffixed-line-length-none Jacobi_MPI_vectormode.F -o jacoby3D.x `

To run the code

  ` mpirun --mca btl ^openib -np 24 ./jacoby3D.x  `

#### tips on MPI performance evaluation: 

get the benchmark:
 
 ` git pull https://github.com/intel/mpi-benchmarks`

Enter in the dir: 
 
 `  cd mpi-benchmarks/src_c `

Compile the code, once you load the module: 
 
 ` make `

Run the executable across two nodes: 

 ` mpirun -np 2  --map-by node --report-bindings  ./IMB-MPI1 PingPong `

Run the executable across two socket: 
 
 ` mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong `

Run the executable within the same socket:

` mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong `

Run the executable using a different protocol: tcp and not infiniband

 `  mpirun -np 2  --mca pml ob1 --mca btl tcp,self --mca btl_tcp_if_include br0  ./IMB-MPI1  PingPong` 

 