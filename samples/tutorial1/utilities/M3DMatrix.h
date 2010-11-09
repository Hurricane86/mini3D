
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_M3DMATRIX_H
#define MINI3D_M3DMATRIX_H

#include <math.h>
#include "M3DVector.h"

	// MatrixInversion, GetMinor and CalcDeterminant code from:
	// http://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/

class M3DMatrix
{
public:
	float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33;

	M3DMatrix() : _00(0), _01(0), _02(0), _03(0), _10(0), _11(0), _12(0), _13(0), _20(0), _21(0), _22(0), _23(0), _30(0), _31(0), _32(0), _33(0) {}

	static M3DMatrix Identity()	{ M3DMatrix m; m._00 = m._11 = m._22 = m._33 = 1; return m; }
	inline M3DMatrix operator * (M3DMatrix& m ) const {	return Multiply(*(M3DMatrix*)this, m); }
	inline float* operator () (unsigned int i, unsigned int j) { return (&_00 + i * 4 + j); };
	inline float* At (unsigned int i, unsigned int j) { return (&_00 + i * 4 + j); };
	inline void SetAt (unsigned int i, unsigned int j, float value) { ((float)*(&_00 + i * 4 + j)) = value; };

	static M3DMatrix Multiply(M3DMatrix& A, M3DMatrix& B)
	{
		M3DMatrix C;
		for(unsigned int i=0; i<4; i++)
   			for(unsigned int j=0; j<4; j++)
      			for(unsigned int k=0; k<4; k++)
       				*C(i,j) += (*A(i,k)) * (*B(k,j));
    
		return C;
	}

	static void LookAt(M3DMatrix& matrix, const M3DVector& eyeVec, const M3DVector& targVec, const M3DVector& upVec)
	{
		M3DVector f = targVec - eyeVec;
		f.Normalize();
	
		M3DVector s = upVec.Cross(f);
		M3DVector u = f.Cross(s);
		s.Normalize();
		u.Normalize();
	
		M3DMatrix LookAt = M3DMatrix::Identity();
		M3DMatrix tra = M3DMatrix::Identity();

		*LookAt(0,0)=s.x; *LookAt(0,1)=s.y; *LookAt(0,2)=s.z;
		*LookAt(1,0)=u.x; *LookAt(1,1)=u.y; *LookAt(1,2)=u.z;
		*LookAt(2,0)=f.x; *LookAt(2,1)=f.y; *LookAt(2,2)=f.z;
		*tra(0,3)=-eyeVec.x; *tra(1,3)=-eyeVec.y; *tra(2,3)=-eyeVec.z;

		LookAt=LookAt*tra;

		for (int i = 0; i < 16; i++)
		{
			*matrix(i % 4, i / 4) = *((&LookAt._00) + i);
		}
	}

	static void PerspectiveProjection(M3DMatrix& matrix, float fov, float aspect, float znear, float zfar)
	{
	  const float h = 1.0f/tan(fov / 2.0f);
	  float neg_depth = znear-zfar;

	  *matrix(0, 0) = h / aspect;
	  *matrix(0, 1) = 0;
	  *matrix(0, 2) = 0;
	  *matrix(0, 3) = 0;

	  *matrix(1, 0) = 0;
	  *matrix(1, 1) = h;
	  *matrix(1, 2) = 0;
	  *matrix(1, 3) = 0;

	  *matrix(2, 0) = 0;
	  *matrix(2, 1) = 0;
	  *matrix(2, 2) = -(zfar + znear)/neg_depth;
	  *matrix(2, 3) = 1;

	  *matrix(3, 0) = 0;
	  *matrix(3, 1) = 0;
	  *matrix(3, 2) = (znear*zfar)/neg_depth;
	  *matrix(3, 3) = 0;
	}

	// matrix inversion, the result is put in Y
	static void MatrixInversion(M3DMatrix* AMat, int order, M3DMatrix* YMat)
	{
		// get the determinant of a
		float det = 1.0f/CalcDeterminant(AMat, 4);

		// memory allocation
		M3DMatrix* minor = new M3DMatrix();		

		for(unsigned int j = 0; j < 4; j++)
		{
			for(unsigned int i = 0; i<4; i++)
			{
				// get the co-factor (matrix) of A(j,i)
				GetMinor(AMat, minor, j, i);
				*YMat->At(i, j) = det * CalcDeterminant(minor, 3);
				if( (i + j) % 2 == 1)
					*YMat->At(i, j) = -*YMat->At(i, j);
			}
		}

		// release memory
		delete minor;
	}

private:

	// calculate the cofactor of element (row,col)
	static int GetMinor(M3DMatrix* srcMat, M3DMatrix *destMat, int row, int col)
	{
		// indicate which col and row is being copied to dest
		int colCount=0,rowCount=0;

		for(int i = 0; i < 4; i++ )
		{
			if( i != row )
			{
				colCount = 0;
				for(int j = 0; j < 4; j++ )
				{
					// when j is not the element
					if( j != col )
					{
						*destMat->At(rowCount, colCount) = *srcMat->At(i,j);
						colCount++;
					}
				}
				rowCount++;
			}
		}

		return 1;
	}

	// Calculate the determinant recursively.
	static float CalcDeterminant(M3DMatrix* pMat, int order)
	{
		// order must be >= 0
		// stop the recursion when matrix is a single element
		if( order == 1 )
			return *pMat->At(0, 0);
		
		// the determinant value
		float det = 0;

		// allocate the cofactor matrix
		M3DMatrix* minor = new M3DMatrix();

		for(int i = 0; i < order; i++ )
		{
			// get minor of element (0,i)
			GetMinor(pMat, minor, 0, i);
			// the recusion is here!
			det += (float)pow(-1.0, i ) * *pMat->At(0, i) * CalcDeterminant(minor, order - 1);
		}

		// release memory
		delete minor;
		return det;
	}

};

#endif
