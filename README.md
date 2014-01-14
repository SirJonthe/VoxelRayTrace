VoxelRayTrace
=============

About
=====

This is a small project that demonstrates discrete ray tracing against a
field of voxels. SDL is used for graphics and event handling.

Building
========

You will need to download and install SDL for your platform in accordance
with the instructions given on this page:
http://lazyfoo.net/SDL_tutorials/lesson01/


The project includes a QtCreator .pro file which can be opened and compiled
directly using GCC and MinGW. The project is simple enough to compile using
any C++ compiler and any IDE. Simply include the .cpp and .h files in a new
project and link the project to SDL and SDLmain (instructions included in the
link above).

Extra
=====

The project has rudimentary OpenMP support. To enable OpenMP you will need to
uncomment "#include <omp.h>" and "omp_set_num_threads(omp_get_num_proc())" in
main.cpp, as well as "#include <omp.h>" and "#pragma omp parallel for" in Renderer.cpp.

Be sure to enable OpenMP in your compiler of choice and link agains the OpenMP
library.
