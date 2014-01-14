#ifndef RENDERER_H_INCLUDED__
#define RENDERER_H_INCLUDED__

#include "mtlList.h"
#include "Voxel.h"
#include "Camera.h"
#include "Ray.h"

class Renderer
{
private:
	mutable byte_t	*m_color;
	int				m_width, m_height;
	bool			m_initialized;
private:
	CollisionInfo GetIntersection(Ray ray, const Voxel *volume, const int dim) const;
public:
			Renderer( void );
	bool	Init(int p_width, int p_height, bool p_fullscreen);
	void	CleanUp( void );
	void	Render(const Camera &camera, const Voxel *volume, const int dim) const;
	void	Refresh( void ) const;
};

#endif
