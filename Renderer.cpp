//#include <omp.h>

#include "PlatformSDL.h"
#include "Ray.h"
#include "Renderer.h"
#include "Math3d.h"

CollisionInfo Renderer::GetIntersection(Ray ray, const Voxel *volume, const int dim) const
{
	CollisionInfo collisionInfo;
	collisionInfo.voxel.isEmpty = true;

	// calculate distances to axis boundries and direction of discrete DDA steps
	int map[3] = { int( ray.origin[0] ), int( ray.origin[1] ), int( ray.origin[2] ) };
	vec3_t deltaDist;
	int step[3];
	for (int i = 0; i < 3; ++i) {
		const float x = (ray.direction[0] / ray.direction[i]);
		const float y = (ray.direction[1] / ray.direction[i]);
		const float z = (ray.direction[2] / ray.direction[i]);
		deltaDist[i] = sqrt( x*x + y*y + z*z );
		if (ray.direction[i] < 0.f) {
			step[i] = -1;
			collisionInfo.impact[i] = (ray.origin[i] - map[i]) * deltaDist[i];
		} else {
			step[i] = 1;
			collisionInfo.impact[i] = (map[i] + 1.f - ray.origin[i]) * deltaDist[i];
		}
	}

	// perform DDA
	while (true) {

		// determine what side dimension should be incremented
		collisionInfo.side = 0;
		for (int i = 1; i < 3; ++i) {
			if (collisionInfo.impact[collisionInfo.side] > collisionInfo.impact[i]) {
				collisionInfo.side = i;
			}
		}
		collisionInfo.impact[collisionInfo.side] += deltaDist[collisionInfo.side];
		map[collisionInfo.side] += step[collisionInfo.side];
		if (map[collisionInfo.side] < 0 || map[collisionInfo.side] >= dim) { break; } // out of bounds

		// sample volume data at calculated position and make collision calculations
		collisionInfo.voxel = volume[map[2]*dim*dim + map[1]*dim + map[0]];
		if (!collisionInfo.voxel.isEmpty) {
			break; // closest voxel is found, no more work to be done
		}
	}

	return collisionInfo;
}

Renderer::Renderer( void ) : m_color(NULL), m_width(0), m_height(0), m_initialized(false) {}

bool Renderer::Init(int p_width, int p_height, bool p_fullscreen)
{
	if (m_initialized) { CleanUp(); }
	m_initialized = (SDL_SetVideoMode(p_width, p_height, 24, SDL_SWSURFACE|SDL_DOUBLEBUF|(p_fullscreen ? SDL_FULLSCREEN : 0)) != NULL);
	if (m_initialized) {
		m_color = (byte_t*)SDL_GetVideoSurface()->pixels;
		m_width = p_width;
		m_height = p_height;
	}
	return m_initialized;
}

void Renderer::CleanUp( void )
{
	if (SDL_GetVideoSurface() != NULL) {
		SDL_FreeSurface(SDL_GetVideoSurface());
		m_color = NULL;
		m_width = 0;
		m_height = 0;
		m_initialized = false;
	}
}

void Renderer::Render(const Camera &camera, const Voxel *volume, const int dim) const
{
	// NOTE: Will not render rays originating from outside a volume correctly. May crash.

	// calculate normals at view port coordinates
	const vec3_t upperLeftNormal	= mml::Normalize(camera.GetPortVector(Camera::PORT_UPPERLEFT) + camera.GetDirection()); // remove +dir later since that locks FOV
	const vec3_t upperRightNormal	= mml::Normalize(camera.GetPortVector(Camera::PORT_UPPERRIGHT) + camera.GetDirection());
	const vec3_t lowerLeftNormal	= mml::Normalize(camera.GetPortVector(Camera::PORT_LOWERLEFT) + camera.GetDirection());
	const vec3_t lowerRightNormal	= mml::Normalize(camera.GetPortVector(Camera::PORT_LOWERRIGHT) + camera.GetDirection());

	const float invHeight	= 1.f / (float)m_height;
	const float invWidth	= 1.f / (float)m_width;

	// left and right y deltas for normal interpolation
	const vec3_t leftNormalDelta	= (lowerLeftNormal - upperLeftNormal) * invHeight;
	const vec3_t rightNormalDelta	= (lowerRightNormal - upperRightNormal) * invHeight;

//#pragma omp parallel for
	for (int y = 0; y < m_height; ++y) {

		const vec3_t leftNormal = upperLeftNormal + leftNormalDelta * y;
		const vec3_t rightNormal = upperRightNormal + rightNormalDelta * y;

		Ray ray;
		ray.origin = camera.GetPosition();
		ray.direction = leftNormal;
		byte_t *pixel = m_color + m_width * y * 3;

		// calculate new x delta
		const vec3_t normalXDelta = (rightNormal - leftNormal) * invWidth;

		for(int x = 0; x < m_width; ++x) {

			CollisionInfo collisionInfo = GetIntersection(ray, volume, dim);

			// draw pixel on screen
			if (!collisionInfo.voxel.isEmpty) {
				pixel[0] = collisionInfo.voxel.rgb[0] >> collisionInfo.side;
				pixel[1] = collisionInfo.voxel.rgb[1] >> collisionInfo.side;
				pixel[2] = collisionInfo.voxel.rgb[2] >> collisionInfo.side;
			} else {
#ifdef _DEBUG
				pixel[color24::r] = (ray.direction[0] < 0.f) ? 255 : 0; // -x = red
				pixel[color24::g] = (ray.direction[1] < 0.f) ? 255 : 0; // -y = green
				pixel[color24::b] = (ray.direction[2] < 0.f) ? 255 : 0; // -z = blue
#else
				pixel[0] = 0;
				pixel[1] = 0;
				pixel[2] = 0;
#endif
			}

			// interpolate x
			ray.direction += normalXDelta;

			// step to next pixel (3 byte channels)
			pixel += 3;
		}
	}
}

void Renderer::Refresh( void ) const
{
	SDL_Flip(SDL_GetVideoSurface());
}
