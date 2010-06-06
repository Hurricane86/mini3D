/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <math.h>

namespace mini3d
{
namespace utilites
{

	class Matrix4
	{
	public:
		float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33;

		Matrix4()
		{
			_00 = _01 = _02 = _03 = _10 = _11 = _12 = _13 = _20 = _21 = _22 = _23 = _30 = _31 = _32 = _33 = 0;
		}

		static Matrix4 Identity()
		{
			Matrix4 m;
			m._00 = m._11 = m._22 = m._33 = 1;
			return m;
		}

		inline Matrix4 operator * (Matrix4& m ) const 
		{
			return Multiply(*(Matrix4*)this, m);
		}

		inline float* operator () (unsigned int i, unsigned int j) { return (&_00 + i * 4 + j); };

		static Matrix4 Multiply(Matrix4& A, Matrix4& B)
		{
			Matrix4 C;
			for(unsigned int i=0; i<4; i++)
   				for(unsigned int j=0; j<4; j++)
      				for(unsigned int k=0; k<4; k++)
       					*C(i,j) += (*A(i,k)) * (*B(k,j));
    
			return C;
		}
	};

	struct Vector3
	{
		float EPSILON;
		float x,y,z;

		Vector3() : EPSILON(0.001f) {	x = y = z = 0.0f; }
		Vector3(float x, float y, float z) : EPSILON(0.001f), x(x), y(y), z(z) { }

		Vector3 operator + (const Vector3& v ) const { return Vector3( x + v.x, y + v.y, z + v.z ); }
		Vector3 operator - (const Vector3& v ) const { return Vector3( x - v.x, y - v.y, z - v.z ); }
		Vector3 operator -() const { return Vector3( -x, -y, -z ); }
		bool operator == (const Vector3& v) const { return (fabs(x - v.x) < EPSILON) &&	(fabs(y - v.y) < EPSILON) && (fabs(z - v.z) < EPSILON); }
		bool operator != (const Vector3& v) const { return ! (*this == v); }
		const Vector3& operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		const Vector3& operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		const Vector3& operator *= (const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

		Vector3 Cross(const Vector3& v) { return Vector3( y * v.z - z * v.y,  z * v.x - x * v.z,  x * v.y - y * v.x ); }
		void Set( float x, float y, float z ) { this->x = x; this->y = y; this->z = z; }
		void Get( float& x, float& y, float& z ) const {x =  this->x; y = this->y; z = this->z; }
		double Norm() const	{ return ( x*x ) + ( y*y ) + (z*z);}
		double Length() const { return sqrt( Norm() ); }
		double Distance( const Vector3& v ) const { return (v - *this).Length(); }
		void Normalize() { float l = Length(); x /= l; y /= l; z /= l;};
	};

class math3d
{
public:
	static void setuplookat(float* matrix4, float* eyeVec3, float* targVec3, float* upVec3)
	{
		Vector3 pos(eyeVec3[0], eyeVec3[1], eyeVec3[2]);
		Vector3 targ(targVec3[0], targVec3[1], targVec3[2]);
		Vector3 upv(upVec3[0], upVec3[1], upVec3[2]);

		Vector3 f = targ - pos;
		f.Normalize();
	
		//right.CrossProduct((targ-pos),up);
		Vector3 s = upv.Cross(f);
		Vector3 u = f.Cross(s);
		s.Normalize();
		u.Normalize();
	
		Matrix4 LookAt = Matrix4::Identity();
		Matrix4 tra = Matrix4::Identity();

		*LookAt(0,0)=s.x; *LookAt(0,1)=s.y; *LookAt(0,2)=s.z;
		*LookAt(1,0)=u.x; *LookAt(1,1)=u.y; *LookAt(1,2)=u.z;
		*LookAt(2,0)=f.x; *LookAt(2,1)=f.y; *LookAt(2,2)=f.z;
		*tra(0,3)=-pos.x; *tra(1,3)=-pos.y; *tra(2,3)=-pos.z;

		LookAt=LookAt*tra;

		for (int i = 0; i < 16; i++)
		{
			matrix4[(i % 4)  * 4 + (i / 4)] = *((&LookAt._00) + i);
		}
	}

	static void BuildPerspProjMat(float* m, float fov, float aspect, float znear, float zfar)
	{
	  const float h = 1.0f/tan(fov / 2.0f);
	  float neg_depth = znear-zfar;

	  m[0] = h / aspect;
	  m[1] = 0;
	  m[2] = 0;
	  m[3] = 0;

	  m[4] = 0;
	  m[5] = h;
	  m[6] = 0;
	  m[7] = 0;

	  m[8] = 0;
	  m[9] = 0;
	  m[10] = -(zfar + znear)/neg_depth;
	  m[11] = 1;

	  m[12] = 0;
	  m[13] = 0;
	  m[14] = (znear*zfar)/neg_depth;
	  m[15] = 0;
	}
};
};
}
