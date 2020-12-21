### MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=/u/area/cozzini/Foundations_of_HPC_2020/day22/MKL-vs-OpenBLAS/OpenBLAS-0.3.13


cpu: gemm_mkl.x gemm_oblas.x


gemm_mkl.x: gemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

gemm_oblas.x: gemm.c
	gcc -DUSE_FLOAT -DOPENBLAS $^ -m64 -I${OPENBLASROOT} -L/${OPENBLASROOT} -lopenblas -lpthread -o $@

clean:
	rm -rf *.x
