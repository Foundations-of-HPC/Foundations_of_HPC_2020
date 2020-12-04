
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define FOR   0
#define TASKS 1

double heavy_work( int N );

double myS;
#pragma omp threadprivate(myS)


int main( int argc, char **argv )
{

  double S1 = 0, S2 = 0;
  int nthreads;
  int N = 10000;

  if ( argc > 1 )
    N = atoi( *(argv+1) );
  
#pragma omp parallel
#pragma omp master
  nthreads = omp_get_num_threads();
  
  printf("using %d threads with N = %d\n\n", nthreads, N);
  
  double timings[2][nthreads];
  double min_timings[2];
  
  // ----------------------------------------------------- random work  
  memset( timings, 0, 2*nthreads*sizeof(double));
  memset( min_timings, 0, 2*sizeof(double));

 #pragma omp parallel shared(N) reduction(+:S2)
  {
    int myid   = omp_get_thread_num();
    int myseed = myid;
    double tstart, tend;
    
    srand( myseed );
    
    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic) reduction(+:S1)
    for( int i = 0; i < N; i++ )
      S1 += heavy_work( 10 + rand_r(&myseed) % 5345 );

    tend = omp_get_wtime();    
    timings[FOR][myid] = tend - tstart;
    
   #pragma omp barrier
    tstart = omp_get_wtime();
   #pragma omp single nowait
    {
      for ( int i = 0; i < N; i++ )
       #pragma omp task
	myS+=heavy_work( 10 + rand_r(&myseed) % 5345 );
    }

    #pragma omp barrier
    S2 = myS;
    tend = omp_get_wtime();    
    timings[TASKS][myid] = tend - tstart;
  }


  printf("S1 = %g, S2 = %g\n", S1, S2 );
  
    for ( int j = 0; j < 2; j++ )
    {
      min_timings[j] = timings[j][0];
      for( int i = 1; i < nthreads; i++)
	{
	  timings[j][0] += timings[j][i];
	  min_timings[j] = (min_timings[j] < timings[j][i]) ? min_timings[j] : timings[j][i];
	}
      timings[j][0] /= nthreads;
    }
  printf("\ntimings for the random work case:\n"
	 "\tFOR:  %9.7g  [ min: %9.7g ]\n"
	 "\tTASKS: %9.7g  [ min: %9.7g ]\n",
	 timings[FOR][0], min_timings[FOR],
	 timings[TASKS][0], min_timings[TASKS] );

  return 0;
}


double heavy_work( int N )
{
  double guess = 3.141572 / 5 * N;
  guess = ( guess > 200 ? 111 : guess);
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }
  return guess;
}
