// Copyright (c) Jonathan Karlsson 2010
// Code may be used freely for non-commercial purposes.
// Author retains the right to use code for commercial purposes.
// Author retains his moral rights under the applicable copyright laws.

#ifndef MML_VECTOR_H_INCLUDED__
#define MML_VECTOR_H_INCLUDED__

#include <stdarg.h>
#include <cmath>

namespace mml
{

	//
	// Vector
	//
	template < int n >
	class Vector
	{
	public:
		static const int Dimension = n;
	private:
		float e[n];
	public:
		//
		// default
		//
		Vector( void ) {
			// do nothing, let initialize to garbage
		}
		//
		// copy
		//
		Vector(const mml::Vector<n> &v) {
			for (int j = 0; j < n; ++j) e[j] = v.e[j];
		}
		//
		// initializer list
		//
		explicit Vector(float e0, ...) {
			va_list vl;
			va_start(vl, e0);
			e[0] = e0;
			for (int j = 1; j < n; ++j) {
				e[j] = (float)va_arg(vl, double);
			}
			va_end(vl);
		}
		//
		// conversion
		//
		template < int size2 >
		explicit Vector(const mml::Vector<size2> &v) {
			if (n < size2) {
				for (int j = 0; j < n; ++j) e[j] = v[j];
			} else {
				int j = 0;
				for (; j < size2; ++j) e[j] = v[j];
				for (; j < n; ++j) e[j] = 0.f;
			}
		}
		//
		// conversion
		//
		explicit Vector(const float * const v) {
			for (int j = 0; j < n; ++j) e[j] = v[j];
		}
	public:
		//
		// index and conversion
		//
		operator float*( void ) { return e; }
		operator const float* const ( void ) const { return e; }
	public:
		//
		// comparison
		//
		bool operator==(const mml::Vector<n> &r) const {
			for (int j = 0; j < n; ++j) {
				if (e[j] != r[j]) {
					return false;
				}
			}
			return true;
		}
		bool operator!=(const mml::Vector<n> &r) const {
			for (int j = 0; j < n; ++j) {
				if (e[j] == r[j]) {
					return false;
				}
			}
			return true;
		}
		static bool Compare(const Vector<n> &u, const Vector<n> &v, float p_tolerance)
		{
			for (int i = 0; i < n; ++i) {
				if (u[i] < v[i]-p_tolerance || u[i] > v[i]+p_tolerance) {
					return false;
				}
			}
			return true;
		}
	public:
		//
		// assignment operators
		//
		mml::Vector<n> &operator=(const mml::Vector<n> &r)
		{
			for (int j = 0; j < n; ++j) e[j] = r.e[j];
			return *this;
		}
		mml::Vector<n> &operator+=(const mml::Vector<n> &r)
		{
			for (int j = 0; j < n; ++j) e[j] += r.e[j];
			return *this;
		}
		mml::Vector<n> &operator-=(const mml::Vector<n> &r)
		{
			for (int j = 0; j < n; ++j) e[j] -= r.e[j];
			return *this;
		}
		mml::Vector<n> &operator*=(const mml::Vector<n> &r)
		{
			for (int j = 0; j < n; ++j) e[j] *= r;
			return *this;
		}
		mml::Vector<n> &operator*=(float r)
		{
			for (int j = 0; j < n; ++j) e[j] *= r;
			return *this;
		}
	public:
		void Clamp(const mml::Vector<n> &min, const mml::Vector<n> &max)
		{
			for (int i = 0; i < n; ++i) {
				if (e[i] > max[i]) {
					e[i] = max[i];
				} else if (e[i] < min[i]) {
					e[i] = min[i];
				}
			}
		}
		void Round( void )
		{
			for (int i = 0; i < n; ++i) {
				e[i] = floor(e[i] + 0.5f);
			}
		}
		void SnapInt( void )
		{
			for (int i = 0; i < n; ++i) {
				e[i] = float(int(e[i]));
			}
		}
		void Zero( void )
		{
			for (int i = 0; i < n; ++i) {
				e[i] = 0.f;
			}
		}
		float Len( void ) const
		{
			float l = 0.f;
			for (int j = 0; j < n; ++j) { l += e[j]*e[j]; }
			return sqrt(l);
		}
		float LenFast( void ) const
		{
			// pure voodoo
			float l = 0.f;
			for (int j = 0; j < n; ++j) { l += e[j]*e[j]; }
			float xhalf = 0.5f*l;
			int i = *(int*)&l;
			i = 0x5f375a86 - (i>>1);
			float x = *(float*)&i;
			x = x*(1.5f-xhalf*x*x);
			return l*x;
		}
		void Normalize( void )
		{
			const float invlen = 1.f/Len();
			for (int j = 0; j < n; ++j) {
				e[j] *= invlen;
			}
		}
		void NormalizeFast( void )
		{
			const float invlen = 1.f/LenFast();
			for (int j = 0; j < n; ++j) {
				e[j] *= invlen;
			}
		}
		bool IsNormalized(const float p_tolerance = 0.f) const
		{
			float unit = 0.f;
			for (int i = 0; i < n; ++i) {
				unit += e[i]*e[i];
			}
			unit -= 1.f;
			return (unit <= p_tolerance && unit >= -p_tolerance);
		}
		void FixDenormals( void )
		{
			for (int i = 0; i < n; ++i) {
				if (fabs(e[i]) < 1e-30f) { e[i] = 0.f; }
			}
		}
		void Swap(int i, int j)
		{
			float t = e[i];
			e[i] = e[j];
			e[j] = t;
		}
	};

}

//
// arithmetic operators
//
template < int n > inline mml::Vector<n> operator+(mml::Vector<n> l, const mml::Vector<n> &r) { return (l+=r); }
template < int n > inline mml::Vector<n> operator-(mml::Vector<n> l, const mml::Vector<n> &r) { return (l-=r); }
template < int n > inline mml::Vector<n> operator*(mml::Vector<n> l, float r) { return (l*=r); }
template < int n > inline mml::Vector<n> operator*(float l, mml::Vector<n> r) { return (r*=l); }
template < int n > inline mml::Vector<n> operator-(mml::Vector<n> v) {
	for (int j = 0; j < n; ++j) { v[j] = -v[j]; }
	return v;
}

namespace mml
{
	//
	// Dot
	//
	template < int n >
	inline float Dot(const mml::Vector<n> &u, const mml::Vector<n> &v)
	{
		float d = 0.f;
		for (int j = 0; j < n; ++j) { d += u[j] * v[j]; }
		return d;
	}

	//
	// Dist
	//
	template < int n >
	inline float Dist(const mml::Vector<n> &u, const mml::Vector<n> &v)
	{
		return (u - v).Len();
	}

	//
	// Ang
	//
	template < int n >
	inline float Ang(const mml::Vector<n> &u, const mml::Vector<n> &v)
	{
		return acos( Dot(u,v) / (u.Len() * v.Len()) );
	}

	//
	// Cross
	//
	inline mml::Vector<3> Cross(const mml::Vector<3> &u, const mml::Vector<3> &v)
	{
		mml::Vector<3> res;
		res[0] = u[1]*v[2] - u[2]*v[1];
		res[1] = u[2]*v[0] - u[0]*v[2];
		res[2] = u[0]*v[1] - u[1]*v[0];
		return res;
	}

	//
	// Normal
	//
	inline mml::Vector<3> SurfaceNormal(const mml::Vector<3> &x, const mml::Vector<3> &y, const mml::Vector<3> &z) // this should be a specialization of Vector<3>
	{
		mml::Vector<3> normal = Cross(y-x, z-x);
		normal.Normalize();
		return normal;
	}

	//
	// Lerp
	//
	template < int n >
	mml::Vector<n> Lerp(const mml::Vector<n> &u, const mml::Vector<n> &v, float d)
	{
		if (d <= 0.0f) {
			return u;
		} else if (d >= 1.0f) {
			return v;
		}
		return u + d * (v - u);
	}

	//
	// Normalize
	//
	template < int n >
	inline mml::Vector<n> Normalize(mml::Vector<n> v)
	{
		v.Normalize();
		return v;
	}

	//
	// NormalizeFast
	//
	template < int n >
	inline mml::Vector<n> NormalizeFast(mml::Vector<n> v)
	{
		v.NormalizeFast();
		return v;
	}

	//
	// Min
	//
	template < int n >
	inline mml::Vector<n> Min(const mml::Vector<n> &x, mml::Vector<n> y)
	{
		for (int i = 0; i < n; ++i) {
			y[i] = (y[i] < x[i]) ? y[i] : x[i];
		}
		return y;
	}

	//
	// Max
	//
	template < int n >
	inline mml::Vector<n> Max(const mml::Vector<n> &x, mml::Vector<n> y)
	{
		for (int i = 0; i < n; ++i) {
			y[i] = (y[i] > x[i]) ? y[i] : x[i];
		}
		return y;
	}
}

#endif
