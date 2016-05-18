#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>


#define NUMBER_AMOUNT      ( 1024L * 1024L * 1024L * 1L )
#define NUMBER_UPPER_BOUND ( 1024L * 64L )
#define SECOND_IN_NANOS    ( 1000L * 1000L * 1000L )


static uint16_t* numbers = NULL;
static uint32_t* buckets = NULL;


#ifdef SPLASH_SINGLE
static void splash_single( void )
{
	fprintf( stdout,  
           "\n\n"
           "          ... oh, look, it's the ...\n\n\n"
           

           "           ▄██████████████▄▐█▄▄▄▄█▌\n"
           "           ██████▌▄▌▄▐▐▌███▌▀▀██▀▀\n"
           "           ████▄█▌▄▌▄▐▐▌▀███▄▄█▌\n"
           "           ▄▄▄▄▄██████████████▀\n\n\n"
           
           
           "          ...::: FAIL WHALE! :::...\n"
           "\n\n"
         );
}
#endif


#ifdef SPLASH_MULTI
static void splash_multi( void )
{
	fprintf( stdout, "\n\n" );
  fprintf( stdout, "          ... oh, look, it's the ...\n\n\n" );
           

  fprintf( stdout, "           ▄██████████████▄▐█▄▄▄▄█▌\n" );
  fprintf( stdout, "           ██████▌▄▌▄▐▐▌███▌▀▀██▀▀\n" );
  fprintf( stdout, "           ████▄█▌▄▌▄▐▐▌▀███▄▄█▌\n" );
  fprintf( stdout, "           ▄▄▄▄▄██████████████▀\n\n\n" );
           
           
  fprintf( stdout, "          ...::: FAIL WHALE! :::...\n" );
  fprintf( stdout, "\n\n" );
}
#endif


#ifdef SORT

static void check_alloc( void* ptr )
{
  if( ptr == NULL )
  {
    fprintf( stderr, "alloc failed, exiting now ...\n" );
    exit( EXIT_FAILURE );
  }
}


static void init_playground( void )
{
  uint32_t i;

  fprintf( stdout, "generating random numbers ...\n" );

  numbers = ( uint16_t* )malloc( NUMBER_AMOUNT * sizeof( uint16_t ) );
  check_alloc( ( void* )numbers );

  srand( time( NULL ) );

  for( i = 0; i < NUMBER_AMOUNT; i++ )
  {
    *( numbers + i ) = ( uint16_t )( rand() % NUMBER_UPPER_BOUND );
  }
  
  fprintf( stdout, "finished generating random numbers ...\n" );
}


static void release_playground( void )
{
  free( ( void* )numbers );
  free( ( void* )buckets );
  numbers = NULL;
  buckets = NULL;
}


static void sort( void )
{
  uint32_t i, j, k, pos;
  
  /* calloc sets the memory region to zero as well ... */
  buckets = ( uint32_t* )calloc( NUMBER_UPPER_BOUND, sizeof( uint32_t ) );
  check_alloc( ( void* )buckets );
  check_alloc( ( void* )numbers );
  
  for( i = 0; i < NUMBER_AMOUNT; i++ )
  {
    ( *( buckets + ( *( numbers + i ) ) ) )++;
  }

  pos = 0; 

  for( j = 0; j < NUMBER_UPPER_BOUND; j++ )
  {
    for( k = 0; k < *( buckets + j ); k++ )
    {
      *( numbers + pos ) = j;
      pos++;
    }
  }
}


static void verify_data( void )
{
  uint32_t i;
  
  fprintf( stdout, "... verifying data ...\n" );
  
  for( i = 1; i < NUMBER_AMOUNT; i++ )
  {
    assert( *( numbers + ( i - 1 ) ) <= *( numbers + i ) );
  }
}


static void timespec_sub( struct timespec* a, struct timespec* b, struct timespec* res )
{
	res->tv_sec = a->tv_sec - b->tv_sec;
	res->tv_nsec = a->tv_nsec - b->tv_nsec;

	if( res->tv_nsec < 0 )
	{
		--( res->tv_sec );
		res->tv_nsec += SECOND_IN_NANOS;
	}
}

#endif


int main( int argc, char** argv )
{
  #ifdef SORT
  struct timespec tstart;
  struct timespec tend;
  struct timespec tdur;
  #endif

  #ifdef SPLASH_SINGLE
  splash_single();
  #endif
  
  #ifdef SPLASH_MULTI
  splash_multi();
  #endif
  
  #ifdef SORT
  init_playground();
  clock_gettime( CLOCK_MONOTONIC, &tstart );
  sort();
  clock_gettime( CLOCK_MONOTONIC, &tend );
  timespec_sub( &tend, &tstart, &tdur );
  fprintf( stdout, "sorting took %lld.%ld seconds.\n",
                   ( long long int )tdur.tv_sec,
                   tdur.tv_nsec / ( 1000 * 1000 ) );
  verify_data();
  release_playground();
  #endif
  
  return EXIT_SUCCESS;
}
