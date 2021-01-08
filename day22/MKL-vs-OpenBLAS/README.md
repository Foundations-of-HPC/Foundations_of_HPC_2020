### Comparing MKL and OpenBLAS on matrix-matrix multiplication 

The goal of this tutorial is to compare performance of two math libraries available on HPC: MKL and OpenBLAS.

We will perform the comparison focusing on the level 3 BLAS function called *gemm*. Such function comes in different flavours, for double precision (dgemm), single precision (sgemm) and also complex single and complex double (cgemm and zgemm).

The first code ``gemm.c`` is a standard *gemm* code, where 3 matrices A,B,C are allocated, A and B are filled and the BLAS routine calculates the matrix-matrix product C=A\*B

The relevant call is 

```
  GEMMCPU(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, m, B, k, beta, C, m);
```
where ``GEMMCPU`` is a macro to easily switch from single precision (``cblas_sgemm``) to double precision (``cblas_dgemm``).

The standard cblas interfaces are

```
  void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)

  void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
```

The first argument ``Order`` specifies wheter we are using column major storage or row major storage.  
``TransA`` and ``TransB`` tell that the matrices should be taken as they are, so not transposed. The rest are the standard GEMM arguments, to perform the operation

```
  C(M,N) = alpha*A(M,K)*B(K,N) + beta*C(K,N)
```

The parameters ``lda``, ``lbd`` and ``ldc`` are the leading dimensions of the matrices, which, since we are using colmajor order, should be the number of rows (lda=M, ldb=K, ldc=M)

To compile and run the code, first submit an interactive job to the queue system

On ORFEO: 
```
 qsub -q gpu -l walltime=2:00:00 -l nodes=1:ppn=24 -I
```

Load the needed module 

```
  module load intel
```  

In ``Makefile``  modify variable OPENBLASROOT with your installation path to OpenBLAS :
```  
OPENBLASROOT=/path_to_OpenBLAS
```  
and set the desired compilation flag (``-DUSE_FLOAT`` / ``-DUSE_DOUBLE``).

Type 

```
make cpu
```

Compilation will make use of the Intel MKL implementation, which is multithreaded. By default this variable has been set by the queue system to the number of cores requested at submission time (``ppn=24`` means ``OMP_NUM_THREADS=24``), but can be changed at runtime. by means of the  environment variable ``OMP_NUM_THREADS``

To run the code simply issue

```
  ./gemm_mkl.x 
  ./gemm_oblas.x

```

With no argument it will calculate a matrix multiplication with M=2000 K=200 and N=1000 with OMP_NUM_THREADS set to number of processor you choose when you submit the the job ( 24 in this case)

You can now use positional argument to specify the size

```
  ./gemm.x 2000 1000 3000 
```

will use M=2000 K=1000 and N=3000, so we will get C(2000,3000) = A(2000,1000)\*B(1000,3000)

You can now play with the code varying the size of the matrix and also the number of threads.


### OpenBLAS library.

We can now download, compile and used OpenBLAS library to compare performance against MKL one.

First get the library and the unpack it: 

``` 
wget https://github.com/xianyi/OpenBLAS/releases/download/v0.3.13/OpenBLAS-0.3.13.tar.gz
tar -xvzf OpenBLAS-0.3.13.tar.gz
cd OpenBLAS-0.3.13

``` 

Compilation is straighforward according to the info provided  [here](https://github.com/xianyi/OpenBLAS/wiki/User-Manual#compile-the-library)

We just need to do this on the computational node and to load the appropriate gnu compiler 

``` 
module load gnu
make GCC=gcc FC=gfortran TARGET=SKYLAKEX
make PREFIX=/u/dssc/$STUDENT/OpenBLASs-0.3.13 install
```
This should be enough: architecture shoud be recognized automatically (as skylakex) and so AVX512 SIMD vector will be used.
At the end of compilation you should see something like that:

```
 OpenBLAS build complete. (BLAS CBLAS LAPACK LAPACKE)

  OS               ... Linux
  Architecture     ... x86_64
  BINARY           ... 64bit
  C compiler       ... GCC        (cmd & version : GNU GCC 9.3.0))
  Fortran compiler ... GFORTRAN  (cmd & version : GNU Fortran (GCC) 9.3.0)
  Library Name     ... libopenblas_skylakexp-r0.3.13.a (Multi-threading; Max num-threads is 24)

```
Include the OpenBLAS library:
```  
export LD_LIBRARY_PATH=/path_to_OpenBlas:$LD_LIBRARY_PATH
```  

#### Proposed Exercise

- Increasing the matrices size up to 20000x20000 (single precision) or 14000x14000 (double precision) analyse the scaling of the GEMM calculation over the possible number of threads

- Keep the number of threads fixed to one and play with different size comparing the two different libraries.


#### More resources


For further information please visit the official openBLAS page:

  - http://www.openblas.net/

#### Results

![MKL_vs_oBLAS](/uploads/7278b8074155d3898c10315f34ecd746/MKL_vs_oBLAS.png)
