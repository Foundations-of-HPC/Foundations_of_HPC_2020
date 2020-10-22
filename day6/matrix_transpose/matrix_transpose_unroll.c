#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include <papi.h>

extern int TESTS_QUIET;

#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)

/* Declared in test_utils.c */
#define PCHECK(e) \
  if (e!=PAPI_OK)						\
    {printf("Problem in papi call, line %d\n",__LINE__); return 1;}

#define NEVENTS 3
#define NRUNS 10
#define L1WORDS (32 * 1024 / 8)  // double capacity of L1
#define L2WORDS (256 * 1024 / 8) // double capacity of L2

int main(int argc, char **argv)
{
  int events[NEVENTS] =
    {
      PAPI_TOT_CYC,/* total cycles */
      PAPI_L1_DCM, /* stalls on L1 cache miss */
      PAPI_L2_DCM, /* stalls on L2 cache miss */
    };
  
  unsigned long long values[NEVENTS];
  PAPI_event_info_t  info, info1;
  const PAPI_hw_info_t *hwinfo = NULL;
  int     N, Nmax, retval;
  double *array, *array_swap;
  struct timespec ts;

  double time, tstart, tstop;
  
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT)
    printf("wrong PAPI initialization: %d instead of %d\n", retval, PAPI_VER_CURRENT);
  
  for (int i=0; i<NEVENTS; i++) {retval = PAPI_query_event(events[i]) ; PCHECK(retval);}

    
  /* declare an array that is more than twice the L2 cache size */
  Nmax           = L2WORDS/2;
  array          = (double*) malloc(Nmax * Nmax * sizeof(double));
  array_swap     = (double*) malloc(Nmax * Nmax * sizeof(double));

  int Nmin  = 9; //8;
  int Nstep = L1WORDS/4; // this is for the power-of-two serie
  //int Nstep = L1WORDS/128;
  for (N = Nmin; N < Nmax; )
    {
      //printf("Run: data set size=%d\n",size);
      
      /* clear the cache by dragging the whole array through it */
      for (int n = 0; n < Nmax*Nmax; n++) array[n] = 0.;
      for (int n = 0; n < Nmax*Nmax; n++) array_swap[n] = 0.;
      
      /* now load the data in the highest cache level that fits */
      for (int n = 0; n < N; n++) array[n] = (double)n;

      unsigned int N2 = N*N;
      double size = NRUNS * (double)N2 * sizeof(double) / (1024*1024);

      int N4 = 4*N/4;
      
      retval = PAPI_start_counters(events, NEVENTS); PCHECK(retval);

      tstart = TCPU_TIME;
      /* run the experiment */
      for (int cc = 0; cc < NRUNS; cc++)
	{
	  for (int i = 0; i < N; i++)
	    {
	      int offset = i*N;
	      int j;
	      for(j = 0; j < N4; j+=4)
		{
		  array_swap[j*N + i] = array[offset + j];
		  array_swap[(j+1)*N + i+1] = array[offset + j+1];
		  array_swap[(j+2)*N + i+2] = array[offset + j+2];
		  array_swap[(j+3)*N + i+3] = array[offset + j+3];
		}
	      for(;j < N; j++)
		array_swap[j*N + i] = array[offset + j];

	    }
	}
      time = (TCPU_TIME - tstart);
      
      retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);

      
      /* printf("size = %d\nTot cycles: %lld\n",size,values[0]); */
      /* printf("cycles per array loc: %9.5f\n",values[0]/(1.*NRUNS*size)); */
      /* printf("L1 misses:\t%lld\nfraction of L1 lines missed:\t%9.5f\n", */
      /* 	     values[1],values[1]/(size/8.)); */
      /* printf("L2 misses:\t%lld\nfraction of L2 lines missed:\t%9.5f\n", */
      /* 	     values[2],values[2]/(size/8.)); */

      printf("size: %d bw: %g cycles: %lld cycles_per_loc: %9.5f L1miss: %lld L1miss_frac: %9.5f L2miss: %lld L2miss_frac: %9.5f\n",
	     N, size / time,
	     values[0], values[0]/(2.*NRUNS*N2),
	     values[1], (double)values[1]/(2.* NRUNS*N2),
	     values[2], (double)values[2]/(2.* NRUNS*N2));

      // this is for the power-of-two serie
      if (N < L1WORDS)
	N*=2;
      else
	N += Nstep;

  }
  free(array);

  return 0;
}
