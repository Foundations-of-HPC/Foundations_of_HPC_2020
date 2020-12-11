
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


#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +     \
                     (double)myts.tv_nsec * 1e-9)

#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)
#endif

// -------------------------------------------------------------

#if defined(TASKS_GRANULARITY )

#define ROUND_N_TO_GRANULARITY {N += (N%TASKS_GRANULARITY);   printf("tasks will be created with granularity: %d\n", TASKS_GRANULARITY);}
#define CREATE_TASKS for ( int i = 0; i < N; i+= TASKS_GRANULARITY )
#define TASK_FOR for ( int JJ = i; JJ < i+4; JJ++ )

#else

#define ROUND_N_TO_GRANULARITY 
#define CREATE_TASKS for ( int i = 0; i < N; i++ )
#define TASK_FOR
#define JJ i

#endif

// -------------------------------------------------------------

#if defined(RANDOMLY_DECREASING)
#define DECREASING_WORK( I ) workload - ((I) + rand_r(&myseed) % (10+((I)/10)))
#else
#define DECREASING_WORK( I ) workload - (I)
#endif

// -------------------------------------------------------------

#define REPETITIONS 10

#define NSTRATEGIES 2
#define FOR         0
#define TASKS       1
char *STRATEGIES_NAMES[] = {"FORloop", "TASKS"};

#define NTIMINGS    2
#define RND_WORK    0
#define DECR_WORK   1
char *TIMINGS_NAMES[] = {"RANDOM work", "DECREASING work"};


// -------------------------------------------------------------

double heavy_work( int N );



int main( int argc, char **argv )
{

  int nthreads;
  int N = 10000;
  int workload = 40000;
  double wtstart, wtend;
  struct  timespec ts;

  
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
  
  double timings[NTIMINGS][NSTRATEGIES][nthreads];
  double wtimings[NTIMINGS][NSTRATEGIES] = {0.0};
  double min_timings[NTIMINGS][NSTRATEGIES] = {0.0};
  double max_timings[NTIMINGS][NSTRATEGIES] = {0.0};

 #if defined(DEBUG)
  unsigned int howmanytasks[nthreads];
 #endif
  
  memset( timings, 0, NTIMINGS*NSTRATEGIES*nthreads*sizeof(double));
 #if defined(DEBUG)
  memset( howmanytasks, 0, nthreads*sizeof(int));
 #endif

  ROUND_N_TO_GRANULARITY ;

  for ( int R = 0; R < REPETITIONS; R++ )
    {
      printf("shot %d/%d.. ", R, REPETITIONS);
      fflush(stdout);
      
      // ----------------------------------------------------- random work, FOR  
      wtstart = CPU_TIME;
     #pragma omp parallel shared(N, workload)
      {
	struct  timespec myts;
	int myid   = omp_get_thread_num();
	int myseed = myid;
	double tstart, tend;
    
	srand( myseed );
	tstart = CPU_TIME_th;
       #pragma omp for schedule(dynamic)
	for( int i = 0; i < N; i++ )
	  //myresults[i] = heavy_work( 10 + rand_r(&myseed) % workload );
	  heavy_work( 10 + rand_r(&myseed) % workload );

	tend = CPU_TIME_th;
	timings[RND_WORK][FOR][myid] += tend - tstart;
      }
      wtend = CPU_TIME;
      wtimings[RND_WORK][FOR] += wtend - wtstart;


      // ----------------------------------------------------- randomly decreasing work, FOR  
      wtstart = CPU_TIME;
     #pragma omp parallel shared(N, workload)
      {
	struct  timespec myts;
	int myid   = omp_get_thread_num();
	int myseed = myid;
	double tstart, tend;
    
	srand( myseed );
	tstart = CPU_TIME_th;

       #pragma omp for schedule(dynamic)
	for( int i = 0; i < N; i++ )
	  heavy_work( DECREASING_WORK(i) );
    

	tend = CPU_TIME_th;
	timings[DECR_WORK][FOR][myid] += tend - tstart;
      }
      wtend = CPU_TIME;
      wtimings[DECR_WORK][FOR] += wtend - wtstart;


      // ----------------------------------------------------- random work, TASKS
      wtstart = CPU_TIME;
     #pragma omp parallel shared(N, workload)
      {
	struct  timespec myts;
	int myid   = omp_get_thread_num();
	int myseed = myid;
	double tstart, tend;
    
	srand( myseed );
	tstart = CPU_TIME_th;
       #pragma omp single nowait
	{
	  CREATE_TASKS
	   #pragma omp task
	    {
	      TASK_FOR
		heavy_work( 10 + rand_r(&myseed) % workload );
	    }
	}
       #pragma omp barrier
    
	tend = CPU_TIME_th;
	timings[RND_WORK][TASKS][myid] += tend - tstart;
      }
      wtend = CPU_TIME;
      wtimings[RND_WORK][TASKS] += wtend - wtstart;
  

      // ----------------------------------------------------- randomly decreasing work, TASKS  
      wtstart = CPU_TIME;
     #pragma omp parallel shared(N, workload)
      {
	struct  timespec myts;
	int myid   = omp_get_thread_num();
	int myseed = myid;
	double tstart, tend;
    
	srand( myseed );
	tstart = CPU_TIME_th;

       #pragma omp single nowait
	{
	  CREATE_TASKS
	   #pragma omp task
	    {
	     #if defined(DEBUG)
	      howmanytasks[omp_get_thread_num()]++;
	     #endif
	      TASK_FOR
		heavy_work( DECREASING_WORK(JJ) );
	    }
      
	}
       #pragma omp barrier
	tend = CPU_TIME_th;
	timings[DECR_WORK][TASKS][myid] += tend - tstart;
      }
      wtend = CPU_TIME;
      wtimings[DECR_WORK][TASKS] += wtend - wtstart;
    }    

  double INV_REP = 1.0 / REPETITIONS;
  for ( int k = 0; k < NTIMINGS; k++ )
    {
      printf("\ntimings %s:\n", TIMINGS_NAMES[k] );
      for ( int j = 0; j < NSTRATEGIES; j++ )
	{	  
	  min_timings[k][j] = timings[k][j][0];
	  max_timings[k][j] = timings[k][j][0];
	  for( int i = 1; i < nthreads; i++)
	    {
	      timings[k][j][0] += timings[k][j][i];
	      min_timings[k][j] = (min_timings[k][j] < timings[k][j][i]) ? min_timings[k][j] : timings[k][j][i];
	      max_timings[k][j] = (max_timings[k][j] > timings[k][j][i]) ? max_timings[k][j] : timings[k][j][i];
	    }
	  timings[k][j][0] /= nthreads;
	  
	  printf("\t%16s :  w-clock %9.7g, avg %9.7g, min: %9.7g, max: %9.7g\n",
		 STRATEGIES_NAMES[j],
		 wtimings[k][j]*INV_REP, timings[k][j][0]*INV_REP, min_timings[k][j]*INV_REP, max_timings[k][j]*INV_REP );
	}
    }
  
 #if defined(DEBUG)
  for ( int t = 0; t < nthreads; t++ )
    printf("thread %d has processed %u tasks\n", t, howmanytasks[t] );
 #endif
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
