#ifndef CAMERA_H_INCLUDED__
#define CAMERA_H_INCLUDED__

#include "MathTypes.h"

class Camera
{
private:
	static const vec3_t	UpVector;
	vec3_t				m_origin;
	// position independent view port corner coordinates
	vec3_t				m_port[4];
	vec3_t				m_direction; // should equal (ul+ur+ll+lr)/4 normalized
public:
	explicit			Camera(int windowWidth, int windowHeight);

	void				SetPortVectors(int windowWidth, int windowHeight);

	const vec3_t &		GetDirection( void ) const;
	const vec3_t &		GetPosition( void ) const;
	const vec3_t &		GetPortVector(int index) const;

	void				Turn(float h, float p);
	void				Move(float forward, float side, float up);

	void				SetPosition(const vec3_t &vec);
public:
	enum
	{
		PORT_UPPERLEFT,
		PORT_UPPERRIGHT,
		PORT_LOWERLEFT,
		PORT_LOWERRIGHT
	};
};


#endif
