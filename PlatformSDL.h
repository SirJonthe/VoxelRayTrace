#ifndef PLATFORMSDL_H_INCLUDED__
#define PLATFORMSDL_H_INCLUDED__

#ifdef _MSC_VER
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#include "SDL.h"
#else
#include <SDL/SDL.h>
#endif

#endif
