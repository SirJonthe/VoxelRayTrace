#ifndef RAY_H_INCLUDED__
#define RAY_H_INCLUDED__

#include "MathTypes.h"
#include "Voxel.h"

struct Ray
{
	vec3_t	origin;
	vec3_t	direction;
};

struct CollisionInfo
{
	vec3_t	impact;		// absolute location of impact
	int		side;		// what side of a voxel was hit (x=0, y=1, z=2)
	Voxel	voxel;		// a copy of the voxel that was hit
};

#endif
