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

#ifndef AURORA_VERTEXDECLARATION_H
#define AURORA_VERTEXDECLARATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

struct VertexDeclaration
{

public:
	// VertexDataType ENUM
	enum VertexDataType { POSITION = 0, COLOR = 1, TEXTURECOORDINATE = 2, NORMAL = 3, BINORMAL = 4, TANGENT = 5, BLENDWEIGHT = 6, BLENDINDICES = 7 };

	// operator overloading
	VertexDeclaration& operator=(const VertexDeclaration &rhs) { copyVertexDeclaration(rhs.vertexDataTypes_, rhs.count_); return *this; }
	VertexDataType operator[](int i) { return vertexDataTypes_[i]; }

	// Constructors
	VertexDeclaration(void) : vertexDataTypes_(0), count_(0) { }
	VertexDeclaration(const VertexDeclaration& vertexDeclaration) : vertexDataTypes_(0), count_(0)	{ (*this) = vertexDeclaration;	}
	VertexDeclaration(const VertexDataType* vertexDataTypes, unsigned int count) : vertexDataTypes_(0), count_(count) { copyVertexDeclaration(vertexDataTypes, count); };
	
	// Destructor
	~VertexDeclaration(void) { delete[] vertexDataTypes_; };
	
	inline VertexDataType* GetVertexDataTypes(unsigned int& count) const { count = count_; return vertexDataTypes_; };
	inline int GetCount(void) const { return count_; };

private:

	void copyVertexDeclaration(const VertexDataType* vertexDataTypes, unsigned int count)
	{
		count_ = count;
		delete[] vertexDataTypes_;
		vertexDataTypes_ = 0;

		if (count_ == 0)
			return;

		vertexDataTypes_ = new VertexDataType[count_];
		memcpy(&vertexDataTypes_[0], &vertexDataTypes[0], count_ * sizeof(VertexDataType));
	}

	VertexDataType* vertexDataTypes_;
	int count_;
};



#endif