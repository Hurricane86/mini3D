
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_M3DVECTOR_H
#define MINI3D_M3DVECTOR_H

#include <math.h>

struct M3DVector
{
	float x,y,z;

	M3DVector() : x(0), y(0), z(0) { }
	M3DVector(float x, float y, float z) : x(x), y(y), z(z) { }

	M3DVector operator + (const M3DVector& v ) const { return M3DVector( x + v.x, y + v.y, z + v.z ); }
	M3DVector operator - (const M3DVector& v ) const { return M3DVector( x - v.x, y - v.y, z - v.z ); }
	M3DVector operator -() const { return M3DVector( -x, -y, -z ); }
	bool operator == (const M3DVector& v) const { float EPSILON = 0.001f; return (fabs(x - v.x) < EPSILON) &&	(fabs(y - v.y) < EPSILON) && (fabs(z - v.z) < EPSILON); }
	bool operator != (const M3DVector& v) const { return ! (*this == v); }
	const M3DVector& operator += (const M3DVector& v) { x += v.x; y += v.y; z += v.z; return *this; }
	const M3DVector& operator -= (const M3DVector& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	const M3DVector& operator *= (const M3DVector& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	M3DVector Cross(const M3DVector& v) const { return M3DVector( y * v.z - z * v.y,  z * v.x - x * v.z,  x * v.y - y * v.x ); }
	void Set( float x, float y, float z ) { this->x = x; this->y = y; this->z = z; }
	void Get( float& x, float& y, float& z ) const {x =  this->x; y = this->y; z = this->z; }
	double Norm() const	{ return ( x*x ) + ( y*y ) + (z*z);}
	double Length() const { return sqrt( Norm() ); }
	double Distance( const M3DVector& v ) const { return (v - *this).Length(); }
	void Normalize() { float l = (float)Length(); x /= l; y /= l; z /= l;};
};

#endif
