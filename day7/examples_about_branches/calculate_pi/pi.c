/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>


/* + -------------------------------------------------------------------------- +
 * |                                                                            |
 * |  PAPI-related section                                                      |
 * |                                                                            |
 * |  everything related to PAPI will be included between                       |
 * |   #ifdef USE_PAPI / #endif clause                                          |
 * |  note that some macros  are defined empty if USE_PAPI                      |
 * |  is not defined                                                            |
 * + -------------------------------------------------------------------------- +
 */


#ifdef USE_PAPI                        /* ----------------------------------------------------- */
#include <papi.h>


#define PAPI_EVENTS_NUM 4                                   /* the number of events we are     _*/
							    /* interested in                    */
                                                            /* this defines the events          */
							    /*   tot # of cycles                */
							    /*   tot # of instructions          */
							    /*   tot # of conditional branches  */
							    /*   tot # of mispredicted con brn  */
							    /*                                  */
							    /*   note: a branch can also no be  */
							    /*         "conditional", i.e. due  */
							    /*         to conditioanl evaluation*/
							    /*         For instance, a function */
							    /*         call is a non conditional*/
							    /*         branch                   */

int         papi_events[PAPI_EVENTS_NUM] = {PAPI_TOT_CYC,		\
					    PAPI_TOT_INS,		\
					    PAPI_BR_CN,			\
					    PAPI_BR_MSP};                
#define CYC 0
#define INS 1
#define BRN 2
#define MBR 3
long long   papi_values[PAPI_EVENTS_NUM] = {0};		    /* this is the counter              */
long long   papi_tot_values[PAPI_EVENTS_NUM] = {0};	    /* this is the accumulator          */
long double papi_stddev_values[PAPI_EVENTS_NUM] = {0.0};    /* this is the accum. for the stddev*/

							    /* defines the macro that check for_*/
							    /* PAPI's result correctness        */
#define PAPI_CHECK_RES( R ) { if ( (R) != PAPI_OK ) {\
      printf("a problem with PAPI (code %d) arise at line %d\n", (R), __LINE__); return (R); } }

							    /* start the counters               */
#define PAPI_START_CNTRS { int res = PAPI_start_counters(papi_events, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }

							    /* stop the cntrs and get the values*/
#define PAPI_STOP_CNTRS  { int res = PAPI_stop_counters(papi_values, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }

							    /* accumulate the counters values   */
#define ACC_PAPI_VALUES { for ( int pp = 0; pp < PAPI_EVENTS_NUM; pp++ ) { \
      papi_tot_values[pp] += papi_values[pp];				\
      papi_stddev_values[pp] += (long double)papi_values[pp]*papi_values[pp]; \
      papi_values[pp] = 0;}}

#else                                  /* USE_PAPI ELSE --------------------------------------- */

#define PAPI_CHECK_RES( R )
#define PAPI_START_CNTRS
#define PAPI_STOP_CNTRS
#define ACC_PAPI_VALUES

#endif                                 /* USE_PAPI END ---------------------------------------- */




#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)


#define NSHOTS 100000000
#define ITER   10




int main ( int argc, char **argv )
{

  int    N = NSHOTS;
  double time_avg = 0;
  double time_std_dev = 0;
  struct timespec ts;

  
  if ( argc > 1 )
    N = atoi( *(argv+1) );


  unsigned long long shot_in = 0;

  srand48(time(NULL));

  for ( int R = 0; R < ITER; R++ )
    {
      double tstart = CPU_TIME;
      PAPI_START_CNTRS;
      for ( int shot = 0; shot < N; shot++ )
	{

	  const double x  = drand48();                      /* draw 2 pseudo-rnd in [0,1]       */
	  const double y  = drand48();
	  
#ifdef VAR1			                            /* VAR 1 -------------------------- */
	  double r  = x*x + y*y;	  

#ifdef CONDITIONAL
	  if( r < 1.0 )
	    shot_in++;
#else
	  shot_in += ( r < 1.0 );
#endif

#elif defined( VAR2 )					    /* VAR 2 -------------------------- */

	  
#ifdef CONDITIONAL
	  if( (x*x + y*y)  < 1.0 )
	    shot_in++;
#else
	  int add = ( (x*x + y*y) < 1.0 );
	  shot_in += add;
#endif

	  
#endif			                                    /* -------------------------------- */

	}
      double elapsed = CPU_TIME - tstart;
      PAPI_STOP_CNTRS;
      ACC_PAPI_VALUES;

      time_avg     += elapsed;
      time_std_dev += elapsed*elapsed;
      
    }

  
  printf("PI is %g\n", (double)shot_in/ITER/N*4.0 );

  time_avg    /= ITER;
  time_std_dev = sqrt(time_std_dev/ITER - time_avg*time_avg);
  printf("\nseconds          : %7.4g +- %6.4g\n", time_avg, time_std_dev);


  

#ifdef USE_PAPI
							    /* process the counters' infos         */
  long double papi_avg[PAPI_EVENTS_NUM];
  for ( int cc = 0; cc < PAPI_EVENTS_NUM; cc++ )
    {
      papi_avg[cc] = ((long double)papi_tot_values[cc]/ N) / ITER;
      papi_stddev_values[cc] = sqrt(((papi_stddev_values[cc] / N) / N) / ITER  - papi_avg[cc]*papi_avg[cc]);
    }


							    /* output useful numbers               */
  
  printf("CPE              : %6s%7.4Lg +- %5.3Lg\n"         /* cycles per element (of the array)   */
	 "IPE              : %13.9Lg +- %7.4Lg\n"	    /* instructions per element            */
	 "BRANCHES_PE      : %13.9Lg +- %7.4Lg\n"	    /* conditional branches per element    */
	 "MISP_BRANCHES_PE : %13.9Lg +- %7.4Lg "	    /* mispredicted cond. brn. per element */
	 " ( tot: %13.9Lg )\n",				    /* total mispredicted cond. branches   */
	 "", papi_avg[CYC], papi_stddev_values[CYC],
	 papi_avg[INS], papi_stddev_values[INS],
	 papi_avg[BRN], papi_stddev_values[BRN],
	 papi_avg[MBR], papi_stddev_values[MBR],
	 papi_avg[MBR]*N);
#endif

  return 0;
}
