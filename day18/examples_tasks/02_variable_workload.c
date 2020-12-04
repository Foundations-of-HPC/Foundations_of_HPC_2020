
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


#define FOR1   0
#define FOR2   2
#define TASKS1 1
#define TASKS2 1

double heavy_work( int N );



int main( int argc, char **argv )
{

  int nthreads;
  int N = 10000;
  int workload = 40000;

  if ( argc > 1 )
    {
      N = atoi( *(argv+1) );
      if ( argc > 2 )
	workload = atoi(*(argv+2));
    }
  
#pragma omp parallel
#pragma omp master
  nthreads = omp_get_num_threads();
  
  printf("using %d threads with N = %d\n\n", nthreads, N);
  
  double timings[4][nthreads];
  double min_timings[2];

  //double *myresults = (double*)calloc( N*nthreads, sizeof(double));
  
  // ----------------------------------------------------- random work  
  memset( timings, 0, 2*nthreads*sizeof(double));
  memset( min_timings, 0, 2*sizeof(double));

 #pragma omp parallel shared(N, workload)
  {
    int myid   = omp_get_thread_num();
    int myseed = myid;
    double tstart, tend;
    
    srand( myseed );

    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic)
    for( int i = 0; i < N; i++ )
      //myresults[i] = heavy_work( 10 + rand_r(&myseed) % workload );
      heavy_work( 10 + rand_r(&myseed) % workload );

    tend = omp_get_wtime();    
    timings[FOR1][myid] = tend - tstart;

    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic)
    for( int i = 0; i < N; i++ )
      //myresults[i] = heavy_work( 10 + rand_r(&myseed) % workload );
      heavy_work( workload - i );

    tend = omp_get_wtime();    
    timings[FOR2][myid] = tend - tstart;

   #pragma omp barrier

    tstart = omp_get_wtime();
   #pragma omp single nowait
    {
      for ( int i = 0; i < N; i++ )
       #pragma omp task
	heavy_work( 10 + rand_r(&myseed) % workload );
    }
    tend = omp_get_wtime();    
    timings[TASKS1][myid] = tend - tstart;

   #pragma omp barrier
    tstart = omp_get_wtime();
   #pragma omp single nowait
    {
      for ( int i = 0; i < N; i++ )
       #pragma omp task
	heavy_work( workload - i );
      
    }

    
    tend = omp_get_wtime();    
    timings[TASKS2][myid] = tend - tstart;

    
  }

  //free(myresults);
  
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
  printf("\ntimings :\n"
	 "\tFOR random       :  %9.7g  [ min: %9.7g ]\n"
	 "\tFOR decreasing   :  %9.7g  [ min: %9.7g ]\n"
	 "\tTASKS random     :  %9.7g  [ min: %9.7g ]\n"
	 "\tTASKS decreasing :  %9.7g  [ min: %9.7g ]\n",
	 
	 timings[FOR1][0], min_timings[FOR1],
	 timings[FOR2][0], min_timings[FOR2],
	 timings[TASKS1][0], min_timings[TASKS1],
	 timings[TASKS2][0], min_timings[TASKS2]);

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
