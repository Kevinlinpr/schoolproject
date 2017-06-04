#include "errors.hpp"
#include <stdio.h>
#include <SDL2/SDL.h>

void fatalError( const char *mes )
{
  printf( "%s\n", mes );
  SDL_Quit();
  /* clean after SOIL ? and other */
}
