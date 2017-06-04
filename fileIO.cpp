#include "fileIO.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

char *readFile( const char *name, const char *read )
{
  FILE *file_in = fopen( name, "r" );
  if( file_in == NULL )
    {
      printf( "Error, file '%s' can't be opened.\n", name );
      return NULL;
    }
  if( fseek( file_in, 0, SEEK_END ) != 0 ) // this method works for files less than 4GB
    {
      printf( "Error, in fseek -> SEEK_END. Make shure file is less than 4GB. File '%s'.\n", name );
      return NULL;
    }
  long int length = ftell( file_in ); // get file length
  if( length == -1L )
    {
      printf( "Error, in ftell. File '%s'.\n", name);
      return NULL;
    }
  if( fseek( file_in, 0, SEEK_SET ) != 0 )
    {
      printf( "Error, in fseek -> SEEK_SET. File '%s'.\n", name );
      return NULL;
    }
  char *cr = (char *)malloc( (length + 1) * sizeof( char ) ); // allocate memory
  if( !cr )
    {
      printf( "Error, could not allocate memory for string.\n" );
      return NULL;
    }
  if( fread( cr, sizeof( char ), length, file_in ) != length ) // get string from file
    {
      printf( "Error, can't read from file '%s'.\n", name );
    }
  fclose( file_in );
  cr[length] = '\0';
  return cr; // return result
}

