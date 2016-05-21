#include <stdlib.h>
#include <stdint.h>

#define LOOP_COUNT 10

int main( void )
{
  uint8_t  i;
  uint32_t dummy;

  dummy = 0;

  for( i = 0; i < LOOP_COUNT; i++ )
  {
    dummy++;
  }

  return EXIT_SUCCESS;
}

