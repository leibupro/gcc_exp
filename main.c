/* ############################################################


    File:          main.c


    Purpose:       parallel implementation
                   of a specific form of
                   the bucket sort algorithm.
    

    Remarks:       - linking requires the
                     pthread library
                     -lpthread

                   - linking requires the
                     realtime library to
                     perform time measurement
                     -lrt 
    

    Author:        P. Leibundgut <leiu@zhaw.ch>
                                 <pl@vqe.ch>


    Date:          05/2016


   ############################################################ */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include "util.h"


#define NUMBER_AMOUNT      ( 1024L * 1024L * 1024L * 1L )
#define NUMBER_UPPER_BOUND ( 1024L * 64L )
#define MAX_THREADS        64


typedef struct
{
  uint16_t  tid;
  pthread_t thread;
  uint16_t* start_addr;
  uint16_t* end_addr;
  uint32_t* buckets;

} thread_container_t;


static uint8_t no_threads        = 0;
static thread_container_t threads[ MAX_THREADS ];

static const uint64_t no_numbers = NUMBER_AMOUNT;
static uint16_t* numbers         = NULL;

static const uint32_t no_buckets = NUMBER_UPPER_BOUND;


/* module local function prototypes */
static void  check_no_threads( void );
static void  init_threads( void );
static void  setup_threads( int argc, char** argv );
static void  partition_tasks( void );
static void  check_alloc( void* ptr );
static void  init_playground( void );
static void  release_playground( void );
static void* bucketize( void* arg ); /* thread function */
static void  sort( void );
static void  verify_data( void );


static void check_no_threads( void )
{
  if( no_threads <= 0 )
  {
    fail_whale( stderr );
    fprintf( stderr, "wrong number of threads. exiting ...\n" );
    exit( EXIT_FAILURE );
  }
}


static void init_threads( void )
{
  uint8_t i;
  for( i = 0; i < no_threads; i++ )
  {
    threads[ i ].tid        = i;
    threads[ i ].start_addr = NULL;
    threads[ i ].end_addr   = NULL;
    threads[ i ].buckets    = NULL;
  }
}


static void setup_threads( int argc, char** argv )
{
  if( argc == 2 )
  {
    no_threads = ( uint8_t )try_strtol( *( argv + 1 ) );
    if( no_threads > MAX_THREADS || no_threads < 1 )
    {
      fail_whale( stderr );
      fprintf( stderr, " specify a valid number of threads.\n"
                       "    range: [1, %d]\n\n",
                       MAX_THREADS );
      exit( EXIT_FAILURE );
    }
  }
  else
  {
    fail_whale( stderr );
    fprintf( stderr, " specify number of threads. range: [1, %d]\n\n",
                     MAX_THREADS );
    exit( EXIT_FAILURE );
  }
  init_threads();
}


static void partition_tasks( void )
{
  uint64_t part_size = 0;
  uint64_t remainder = 0;
  uint8_t  i;
 
  check_no_threads(); 

  part_size = no_numbers / no_threads;
  remainder = no_numbers % no_threads;

  check_alloc( ( void* )numbers );
  
  for( i = 0; i < no_threads; i++ )
  {
    threads[ i ].start_addr = ( uint16_t* )( numbers + i * part_size );
    threads[ i ].end_addr = 
      ( uint16_t* )( numbers + i * part_size + part_size );
  }

  if( remainder > 0 )
  {
    threads[ no_threads - 1 ].end_addr += remainder; 
  }
}


static void check_alloc( void* ptr )
{
  if( ptr == NULL )
  {
    fail_whale( stderr );
    fprintf( stderr, "alloc failed, exiting now ...\n" );
    exit( EXIT_FAILURE );
  }
}


static void init_playground( void )
{
  uint64_t i;

  fprintf( stdout, "generating random numbers ...\n" );

  numbers = ( uint16_t* )malloc( no_numbers * sizeof( uint16_t ) );
  check_alloc( ( void* )numbers );

  srand( time( NULL ) );

  for( i = 0; i < no_numbers; i++ )
  {
    *( numbers + i ) = ( uint16_t )( rand() % no_buckets );
  }
  
  fprintf( stdout, "finished generating random numbers ...\n" );
}


static void release_playground( void )
{
  uint8_t i;

  for( i = 0; i < no_threads; i++ )
  {
    free( ( void* )threads[ i ].buckets );
    threads[ i ].buckets = NULL;
  }

  free( ( void* )numbers );
  numbers = NULL;
}


/* this is our thread function ... */
static void* bucketize( void* arg )
{
  uint8_t   tid = *( ( uint8_t* )arg );
  uint16_t* start_addr = threads[ tid ].start_addr;
  uint16_t* end_addr = threads[ tid ].end_addr;
  uint32_t* buckets = NULL;
  uint16_t* p = NULL;

  /* calloc sets the memory region to zero as well ... */
  buckets = ( uint32_t* )calloc( no_buckets, sizeof( uint32_t ) );
  check_alloc( ( void* )buckets );
  threads[ tid ].buckets = buckets;
  
  
  for( p = start_addr; p < end_addr; p++ )
  {
    ( *( buckets + ( *p ) ) )++;
  }
  pthread_exit( NULL );
}


static void sort( void )
{
  uint32_t i, j, k, pos;
  uint32_t* buckets_sumup = NULL;
  
  check_no_threads();
  partition_tasks();

  for( i = 0; i < no_threads; i++ )
  {
    if( pthread_create( &threads[ i ].thread, 
                        NULL, 
                        bucketize,
                        ( void* )&( threads[ i ].tid ) ) 
        != 0 )
    {
      fail_whale( stderr );
      fprintf( stderr, "failed to create child thread. exiting ...\n" );
      exit( EXIT_FAILURE );
    }
  }

  for( i = 0; i < no_threads; i++ )
  {
    pthread_join( threads[ i ].thread, NULL );
  }
  
  /* we have to sum up all buckets from all threads,
   * if we have more than one thread */
  if( no_threads > 1 )
  {
    /* calloc sets the memory region to zero as well ... */
    buckets_sumup = ( uint32_t* )calloc( no_buckets, sizeof( uint32_t ) );
    check_alloc( ( void* )buckets_sumup );

    for( i = 0; i < no_buckets; i++ )
    {
      for( j = 0; j < no_threads; j++ )
      {
        *( buckets_sumup + i ) += *( threads[ j ].buckets + i );
      }
    }
  }
  else if( no_threads == 1 )
  {
    buckets_sumup = threads[ 0 ].buckets;
  }

  pos = 0; 

  for( j = 0; j < no_buckets; j++ )
  {
    for( k = 0; k < *( buckets_sumup + j ); k++ )
    {
      *( numbers + pos ) = j;
      pos++;
    }
  }

  if( no_threads > 1 )
  {
    free( ( void* )buckets_sumup );
    buckets_sumup = NULL;
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


int main( int argc, char** argv )
{
  struct timespec tstart;
  struct timespec tend;
  struct timespec tdur;
  
  setup_threads( argc, argv );
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
  
  return EXIT_SUCCESS;
}

