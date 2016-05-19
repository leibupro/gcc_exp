/* ############################################################

    
    File:          util.c

    
    Purpose:       util module with generic
                   functions in it, which
                   can be used in any given
                   project.
    

    Remarks:       - consult man pages for
                     explanation of function
                     details.


    Author:        P. Leibundgut <leiu@zhaw.ch>


    Date:          11/2015


   ############################################################ */

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "util.h"


long try_strtol( char* str )
{
  long val;
  char* endptr;

  errno = 0;
  
  val = strtol( str, &endptr, 10 );

  /* Check for various possible errors */
  if( ( errno == ERANGE && 
        ( val == LONG_MAX || val == LONG_MIN ) )
      || ( errno != 0 && val == 0 ) ) 
  {
    fprintf( stderr, "strtol failed, exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  if( endptr == str )
  {
    fprintf( stderr, "No digits were found, exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  return val;
}


void timespec_sub( struct timespec* a, struct timespec* b, struct timespec* res )
{
  res->tv_sec  = a->tv_sec  - b->tv_sec;
  res->tv_nsec = a->tv_nsec - b->tv_nsec;
  
  if( res->tv_nsec < 0 )
  {
    --( res->tv_sec );
    res->tv_nsec += SECOND_IN_NANOS;
  }
}
