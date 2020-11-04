

#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <time.h>
#include "reduction.h"

#include "mypapi.h"

#define N_default         1000000
#define simple            0 
#define unroll2_1acc      1
#define unroll2_1acc_swap 2
#define unroll2           3
#define unroll4           4
#define ITER              10
#define ALIGN             32

#if !defined(OP)
#define OP +
#endif

DATYPE scan         ( const uLint, DTYPE * restrict );
DATYPE scan_unroll2 ( const uLint, DTYPE * restrict );
DATYPE scan_unroll4 ( const uLint, DTYPE * restrict );
DATYPE scan_unroll2_1accumulator( const uLint, DTYPE * restrict );
DATYPE scan_unroll2_1accumulator_swapped( const uLint, DTYPE * restrict );

inline DATYPE scan( const uLint N, DTYPE * restrict array )
{

  DATYPE acc = 0;
  for ( uLint ii = 1; ii < N; ii++ )
    acc = acc OP array[ii];
  
  return acc;
}

inline DATYPE scan_unroll2_1accumulator( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc = 0;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 2; ii < N_2; ii+=2 )
    {
      acc = (acc OP array[ii]) OP array[ii+1];
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc = acc OP array[ii];

  return acc;
}


inline DATYPE scan_unroll2_1accumulator_swapped( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc = 0;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 2; ii < N_2; ii+=2 )
    {
      acc = acc OP (array[ii] OP array[ii+1]);
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc = acc OP array[ii];

  return acc;
}


inline DATYPE scan_unroll2( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc[2] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 2; ii < N_2; ii+=2 )
    {
      acc[0] = acc[0] OP array[ii];
      acc[1] = acc[0] OP array[ii+1];
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc[0] = acc[0] OP array[ii];
  acc[0] = acc[0] OP acc[1];
  
  return acc[0];
}


inline DATYPE scan_unroll4( const uLint N, DTYPE * restrict array )
{

  uLint    N_4  = (N/4)*4;

  DATYPE acc[4] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 4; ii < N_4; ii+=4 )
    {
      acc[0] = acc[0] OP array[ii];
      acc[1] = acc[1] OP array[ii+1];
      acc[2] = acc[2] OP array[ii+2];
      acc[3] = acc[3] OP array[ii+3];
    }

  acc[1] = acc[1] OP (acc[2] OP acc[3]);
  for ( uLint ii = N_4; ii < N; ii++ )
    acc[0] = acc[0] OP array[ii];
  acc[0] = acc[0] OP acc[1];
  
  return acc[0];
}



int main ( int argc, char **argv )
{
  
  struct timespec ts;  
  int             Nth_level1 = 1;
  int             Nth_level2 = 0;
  
  // -------------------------------------------------------------
  // variables' initialization to default values
  //

  int    N         = N_default;
  int    scan_type = simple;
  
  
  if ( argc > 1 )
    {
      scan_type = atoi( *(argv+1) );
      if ( argc > 2 )
	N  = atoi( *(argv+2) );
    }

  printf( "scan type: %d\n", scan_type );

  
  // -------------------------------------------------------------
  // data init.

  double timing_start;
  double timing_scan = 0;
  double timing_scan_stddev = 0;
  double timing_prepare;
  double total_weight;
  
  int    N_alloc = ((N/4)+1)*4;
  DTYPE *array   = (DTYPE*)aligned_alloc( ALIGN, N_alloc * sizeof(DTYPE) );

  PAPI_INIT;

  timing_start = CPU_TIME; 

  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (DTYPE)ii;

  for(int k = 0; k < ITER; k++)
    {
      
      PAPI_START_CNTR;
      timing_prepare = CPU_TIME;
      
      if ( scan_type == simple ) 
	
	total_weight = scan( N, array );

      if ( scan_type == unroll2_1acc ) 

	total_weight = scan_unroll2_1accumulator( N, array );

      if ( scan_type == unroll2_1acc_swap ) 

	total_weight = scan_unroll2_1accumulator_swapped( N, array );

      else if (scan_type == unroll2)
	
	total_weight = scan_unroll2( N, array );
      
      else if (scan_type == unroll4)
	
	total_weight = scan_unroll4( N, array );
      
      double deltat = CPU_TIME-timing_prepare;
      timing_scan        += deltat;
      timing_scan_stddev += deltat*deltat;
      PAPI_STOP_CNTR;
    }

  timing_scan /= ITER;
  
  printf("timing for scan is %g, timing for prepare is %g [total weight: %g]\n",
	 timing_scan, timing_prepare-timing_start, total_weight);

 #ifdef USE_PAPI
  printf( "\tIPC: %4.2g\n"
          "\t[ time-per-element: %6.4gsec  -  cycles-per-element: %9.6g ]\n",
          (double)papi_values[0] / papi_values[1],
	  timing_scan / N,
          (double)papi_values[1] / (ITER*N) );
 #endif

  return 0;
}
