// Copyright (c) Jonathan Karlsson 2010
// Code may be used freely for non-commercial purposes.
// Author retains the right to use code for commercial purposes.
// Author retains his moral rights under the applicable copyright laws.

#ifndef MML_MATH3D_H_INCLUDED__
#define MML_MATH3D_H_INCLUDED__

#include <cmath>
#include "Vector.h"
#include "Matrix.h"

/* constants
========================================*/
// NOTE: For some reason MSVC fails to initialize some constants (instead, initializes them to 0) if constants they depend on
// are not magic numbers or defines.

#ifdef PI
#undef PI
#endif
#define PI 3.14159265f

#ifdef DEG_MIN
#undef DEG_MIN
#endif
#define DEG_MIN 0.f

#ifdef DEG_MAX
#undef DEG_MAX
#endif
#define DEG_MAX 360.f

#ifdef RAD_MIN
#undef RAD_MIN
#endif
#define RAD_MIN 0.f

#ifdef RAD_MAX
#undef RAD_MAX
#endif
#define RAD_MAX PI*2.f

//static const float PI = 3.14159265f;
//static const float DEG_MIN = 0.f;
//static const float DEG_MAX = 360.f;
static const float DEG_QUAD = DEG_MAX/4.f;
static const float DEG_QUAD1 = DEG_QUAD*0.f;
static const float DEG_QUAD2 = DEG_QUAD*1.f;
static const float DEG_QUAD3 = DEG_QUAD*2.f;
static const float DEG_QUAD4 = DEG_QUAD*3.f;
//static const float RAD_MIN = 0.f;
//static const float RAD_MAX = PI*2.f;
static const float RAD_QUAD = RAD_MAX/4.f;
static const float RAD_QUAD1 = RAD_QUAD*0.f;
static const float RAD_QUAD2 = RAD_QUAD*1.f;
static const float RAD_QUAD3 = RAD_QUAD*2.f;
static const float RAD_QUAD4 = RAD_QUAD*3.f;

/* general mathematical functions
========================================*/
inline float DegToRad(float pDeg) { return pDeg *((RAD_QUAD*2.f)/(DEG_QUAD*2.f)); }
inline float RadToDeg(float pRad) { return pRad *((DEG_QUAD*2.f)/(RAD_QUAD*2.f)); }
template < typename T > inline const T &Min2(const T &pA, const T &pB) { return (pA < pB) ? (pA) : (pB); }
template < typename T > inline const T &Max2(const T &pA, const T &pB) { return (pA > pB) ? (pA) : (pB); }
template < typename T > inline const T &Min3(const T &pA, const T &pB, const T &pC) { return Min2(Min2(pA,pB),pC); }
template < typename T > inline const T &Max3(const T &pA, const T &pB, const T &pC) { return Max2(Max2(pA,pB),pC); }
template < typename T > inline T &Min2(T &pA, T &pB) { return (pA < pB) ? (pA) : (pB); }
template < typename T > inline T &Max2(T &pA, T &pB) { return (pA > pB) ? (pA) : (pB); }
template < typename T > inline T &Min3(T &pA, T &pB, T &pC) { return Min2(Min2(pA,pB),pC); }
template < typename T > inline T &Max3(T &pA, T &pB, T &pC) { return Max2(Max2(pA,pB),pC); }
template < typename T > inline void Swap(T &pA, T&pB) { T temp = pA; pA = pB; pB = temp; }
inline float FastInvSqrt(float pX)
{
	float xhalf = 0.5f*pX;
	int i = *(int*)&pX; // get bits for floating value
	i = 0x5f375a86 - (i>>1); // gives initial guess y0
	pX = *(float*)&i; // convert bits back to float
	pX = pX*(1.5f-xhalf*pX*pX); // Newton step, repeating increases accuracy

	// Due to a very good initial guess, no iterations are needed to recieve a really accurate result.
	// Initial guess: 0x5f375a86 (magic number)
	return pX;
}
inline float FastSqrt(float pX) { return pX*FastInvSqrt(pX); }

//
// TranslationMatrix
//
inline mml::Matrix<4,4> TranslationMatrix(float x, float y, float z)
{
	return mml::Matrix<4,4>(
		1.f, 0.f, 0.f, x,
		0.f, 1.f, 0.f, y,
		0.f, 0.f, 1.f, z,
		0.f, 0.f, 0.f, 1.f
		);
}

//
// ScaleMatrix
//
inline mml::Matrix<4,4> ScaleMatrix(float x, float y, float z)
{
	return mml::Matrix<4,4>(
		x,   0.f, 0.f, 0.f,
		0.f, y,   0.f, 0.f,
		0.f, 0.f, z,   0.f,
		0.f, 0.f, 0.f, 1.f
		);
}

//
// NormalTransformMatrix
//
inline mml::Matrix<3,3> NormalTransformMatrix(const mml::Matrix<4,4> &mat)
{
	return mml::Matrix<3,3>(Transp(Inv(mat)));
}

//
// TBNMatrix
//
inline mml::Matrix<4,4> TBNMatrix(const mml::Vector<3> &t, const mml::Vector<3> &b, const mml::Vector<3> &n)
{
	// the previous version ordered it differently
	// tx, ty, tz
	// bx, by, bz
	// nx, ny, nz
	// was this wrong, or was the choise conscious?
	return mml::Matrix<4,4>(
		t[0], b[0], n[0], 0.f,
		t[1], b[1], n[1], 0.f,
		t[2], b[2], n[2], 0.f,
		0.f,  0.f,  0.f,  1.f
		);
}

//
// XRotationMatrix
//
inline mml::Matrix<4,4> XRotationMatrix(float xrad)
{
	const float SIN = sin(xrad);
	const float COS = cos(xrad);
	return mml::Matrix<4,4>(
		1.f, 0.f,  0.f, 0.f,
		0.f, COS, -SIN, 0.f,
		0.f, SIN,  COS, 0.f,
		0.f, 0.f,  0.f, 1.f
		);
}

//
// YRotationMatrix
//
inline mml::Matrix<4,4> YRotationMatrix(float yrad)
{
	const float SIN = sin(yrad);
	const float COS = cos(yrad);
	return mml::Matrix<4,4>(
		 COS, 0.f, SIN,	0.f,
		 0.f, 1.f, 0.f, 0.f,
		-SIN, 0.f, COS,	0.f,
		 0.f, 0.f, 0.f, 1.f
		);
}

//
// ZRotationMatrix
//
inline mml::Matrix<4,4> ZRotationMatrix(float zrad)
{
	const float SIN = sin(zrad);
	const float COS = cos(zrad);
	return mml::Matrix<4,4>(
		COS, -SIN, 0.f, 0.f,
		SIN,  COS, 0.f, 0.f,
		0.f,  0.f, 1.f, 0.f,
		0.f,  0.f, 0.f, 1.f
		);
}

//
// EulerRotationMatrix
//
inline mml::Matrix<4,4> EulerRotationMatrix(float head, float pitch, float roll)
{
	const float
		SINH = sin(head),
		SINP = sin(pitch),
		SINR = sin(roll),
		COSH = cos(head),
		COSP = cos(pitch),
		COSR = cos(roll);
	return mml::Matrix<4,4>(
		(COSR*COSH) -(SINR*SINP*SINH), -SINR*COSP, (COSR*SINH) +(SINR*SINP*COSH), 0.f,
		(SINR*COSH) +(COSR*SINP*SINH),  COSR*COSP, (SINR*SINH) -(COSR*SINP*COSH), 0.f,
		-COSP*SINH,                     SINP,      COSP*COSH,                     0.f,
		0.f,                            0.f,       0.f,                           1.f
		);
}

// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/the-theory-of-stencil-shadow-volumes-r1873
// Should apparently be used for stencil shadows using z-fail method
inline mml::Matrix<4,4> InfPerspectiveMatrix(float fovh, float fovv, float near, float far)
{
	return mml::Matrix<4,4>(
		1.f/tan(fovh/2.f), 0.f, 0.f, 0.f,
		0.f, 1.f/tan(fovv/2.f), 0.f, 0.f,
		0.f, 0.f, far/(far-near), 1.f,
		0.f, 0.f, (-far*near)/(far-near), 0.f
		);
}

inline mml::Matrix<4,4> AxisRotationMatrix(const mml::Vector<3> &p_axis, float p_rot)
{
	const float x = p_axis[0];
	const float y = p_axis[1];
	const float z = p_axis[2];
	const float c = cosf(p_rot);
	const float s = sinf(p_rot);

	return mml::Matrix<4,4>(
		(x * x) * (1.0f - c) + c,       (x * y) * (1.0f - c) + (z * s), (x * z) * (1.0f - c) - (y * s), 0.f,
		(y * x) * (1.0f - c) - (z * s), (y * y) * (1.0f - c) + c,       (y * z) * (1.0f - c) + (x * s), 0.f,
		(z * x) * (1.0f - c) + (y * s), (z * y) * (1.0f - c) - (x * s), (z * z) * (1.0f - c) + c,       0.f,
		0.f,                            0.f,                            0.f,                            1.f
	);
}

inline mml::Matrix<4,4> ViewMatrix(const mml::Vector<3> &p_xAxis, const mml::Vector<3> &p_yAxis, const mml::Vector<3> &p_zAxis, const mml::Vector<3> &p_viewPos)
{
	return mml::Matrix<4,4>(
		 p_xAxis[0],               p_yAxis[0],               p_zAxis[0],              0.f,
		 p_xAxis[1],               p_yAxis[1],               p_zAxis[1],              0.f,
		 p_xAxis[2],               p_yAxis[2],               p_zAxis[2],              0.f,
		-Dot(p_xAxis, p_viewPos), -Dot(p_yAxis, p_viewPos), -Dot(p_zAxis, p_viewPos), 1.f
	);
}

inline mml::Matrix<4,4> ViewMatrix(const mml::Vector<3> &p_up, const mml::Vector<3> &p_target, const mml::Vector<3> p_viewPos)
{
	const mml::Vector<3> zaxis = Normalize(p_target - p_viewPos);
	const mml::Vector<3> xaxis = Normalize(Cross(p_up, zaxis));
	const mml::Vector<3> yaxis = Cross(zaxis, xaxis);
	return ViewMatrix(xaxis, yaxis, zaxis, p_viewPos);
}

#endif
