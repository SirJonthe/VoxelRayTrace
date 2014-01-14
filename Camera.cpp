#include "Math3d.h"
#include "Camera.h"

const vec3_t Camera::UpVector(0.f, 1.f, 0.f);

Camera::Camera(int windowWidth, int windowHeight)
{
	// Problem: does not depend on aspect ratio

	SetPortVectors(windowWidth, windowHeight);

	//
	// -----
	// |  (/ = 180 - 90 - fov/2
	// |  /
	// | /
	// |/ = fov/2
	//
	m_origin[0] = 0.f;
	m_origin[1] = 0.f;
	m_origin[2] = 0.f; // hard-code for now, disregards specified fov

	m_direction[0] = 0.f;
	m_direction[1] = 0.f;
	m_direction[2] = -1.f;
}

void Camera::SetPortVectors(int windowWidth, int windowHeight)
{
	// FOV is implemented by moving port away from origo
	float ratio = float( windowHeight ) / float( windowWidth );
	m_port[PORT_UPPERLEFT][0] = -1.f;
	m_port[PORT_UPPERLEFT][1] = -ratio;
	m_port[PORT_UPPERLEFT][2] = 0.f;

	m_port[PORT_UPPERRIGHT][0] = 1.f;
	m_port[PORT_UPPERRIGHT][1] = -ratio;
	m_port[PORT_UPPERRIGHT][2] = 0.f;

	m_port[PORT_LOWERLEFT][0] = -1.f;
	m_port[PORT_LOWERLEFT][1] = ratio;
	m_port[PORT_LOWERLEFT][2] = 0.f;

	m_port[PORT_LOWERRIGHT][0] = 1.f;
	m_port[PORT_LOWERRIGHT][1] = ratio;
	m_port[PORT_LOWERRIGHT][2] = 0.f;
}

const vec3_t &Camera::GetDirection( void ) const
{
	return m_direction;
}

const vec3_t &Camera::GetPosition( void ) const
{
	return m_origin;
}

void Camera::Turn(float h, float p)
{
	mat4_t m = EulerRotationMatrix(h, p, 0.f);
	for (int i = 0; i < 4; ++i) {
		m_port[i] *= m;
	}
	m_direction *= m;
	m_direction.Normalize(); // re-normalize, just in case
}

void Camera::Move(float forward, float side, float up)
{
	m_origin += m_direction * forward + Camera::UpVector * up;
	m_origin += Cross(Camera::UpVector, m_direction) * side;
}

const vec3_t &Camera::GetPortVector(int index) const
{
	return m_port[index];
}

void Camera::SetPosition(const vec3_t &vec)
{
	m_origin = vec;
}
