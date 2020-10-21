
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

/* ----------------------------------------------------------------- *
 *                                                                   *
 * a very basic example of stack smashing                            *
 *                                                                   *
 * ----------------------------------------------------------------- */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main( void )
{
  register long long int myRBP __asm__("rbp");
  register long long int myRSP __asm__("rsp");
  int array[ 3 ];
  int myprotection[3];
	
  printf("RSP       : %p\n"
	 "RBP       : %p\n"
	 "array     : %p %p\n"
	 "protection: %p %p\n",
	 (void*)myRSP, (void*)myRBP,
	 (void*)&array[0], (void*)&array[1],
         (void*)&myprotection[0], (void*)&myprotection[1]);
  
  for ( int ii = 0; ii <= 3; ii++ )
    {
      printf("accessing %d-th element: %p\n", ii, &array[ii] );
      array[ ii ] = ii;
    }

  printf("everything went well\n");
  
  return 0;
}
