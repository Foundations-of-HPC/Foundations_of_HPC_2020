### Stream makefile for different compiler and options  
###
###
# Intel compilation 

all: stream_intel_omp.x stream_gnu_omp.x  

SIZE=88000000 

# Intel compilation 
stream_intel_omp.x: stream.c 
	icc -DSTREAM_ARRAY_SIZE=$(SIZE) -DNTIMES=200 -Ofast -xCORE-AVX512 -qopt-prefetch-distance=64,8 -qopt-streaming-cache-evict=0 -qopt-streaming-stores always -qopenmp -ffreestanding  $^  -o $@

# Gnu compilation 
stream_gnu_omp.x: stream.c
	gcc -DSTREAM_ARRAY_SIZE=$(SIZE)  -Ofast -fargument-noalias -ffreestanding -mavx512f -fopenmp $^  -o $@

clean:
	rm -rf *.x
