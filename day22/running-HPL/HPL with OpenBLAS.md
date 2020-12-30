#OpenBLAS library

OpenBLAS is an optimized Basic Linear Algebra Subprograms (BLAS) library based on GotoBLAS2 1.13 BSD version.

The BLAS (Basic Linear Algebra Subprograms) are routines that provide standard building blocks for performing basic vector and matrix operations. The Level 1 BLAS perform scalar, vector and vector-vector operations, the Level 2 BLAS perform matrix-vector operations, and the Level 3 BLAS perform matrix-matrix operations. Because the BLAS are efficient, portable, and widely available, they are commonly used in the development of high quality linear algebra software.

## Download and build OpenBLAS

Get the package and unpack it in some location on your home directory, then create the install folder

    wget https://github.com/xianyi/OpenBLAS/releases/download/v0.3.13/OpenBLAS-0.3.13.tar.gz
    tar -xvzf OpenBLAS-0.3.13.tar.gz
    cd OpenBLAS-0.3.13.tar.gz
	mkdir ../openblas-0.3.13

Now get a node on the cluster and compile the library with the appropriate target CPU, in our case SKYLAKEX. 
We also define the variable `PREFIX` with the absolute path of the install dir we have just created:

    make TARGET=SKYLAKEX PREFIX=/u/dssc/$STUDENT/openblas-0.3.13
	cd
    
## Link OpenBLAS to HPL

Go in the HPL directory and copy the Makefile from the previous exercise:

    cd hpl-2.3
    cp Make.Linux_Intel_64 Make.OpenBLAS

We now edit Make.OpenBLAS:
1.  We set the architecture to OpenBLAS so it doesn't overwrite our previous installation:

        ARCH         = OpenBLAS

2. Then we edit the `LAdir` variable equal to the path of the OpenBLAS library:

        LAdir         = /u/dssc/$STUDENT/openblas-0.3.13
    
3. Now we change the `LAlib` variable accordingly:


    LAlib         = $(LAdir)/lib/libopenblas.a
    
Now we load the appriopriate module and compile the HPL benchmark:

    module load intel/20.1
	make arch=OpenBLAS

We will find the xhpl executable in the `~/hpl-2.3/bin/OpenBLAS/` folder.
