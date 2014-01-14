#ifndef VOXEL_H_INCLUDED__
#define VOXEL_H_INCLUDED__

#include "PlatformSDL.h"

typedef unsigned char byte_t;

struct color24
{
#ifdef __MACOSX__
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		enum { r, g, b };
	#else
		enum { b, g, r };
	#endif
#elif defined __WIN32__ || defined __LINUX__
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		enum { b, g, r };
	#else
		enum { r, g, b };
	#endif
#endif

};

// g++ hangs when using constructors to initialize robot model array
#ifdef __MACOSX__
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		#define RGB(r, g, b) { r, g, b }
	#else
		#define RGB(r, g, b) { b, g, r }
	#endif
#elif defined __WIN32__ || defined __LINUX__
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		#define RGB(r, g, b) { b, g, r }
	#else
		#define RGB(r, g, b) { r, g, b }
	#endif
#endif

struct Voxel
{
	byte_t	rgb[3];
	bool	isEmpty;
};

#endif
