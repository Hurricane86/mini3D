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

	enum VertexDataType
	{
		POSITION,
		COLOR,
		TEXTURECOORDINATE,
		NORMAL,
		BINORMAL,
		TANGENT,
		BLENDWEIGHT,
		BLENDINDICES
	};

public: VertexDeclaration& operator=(const VertexDeclaration &rhs)
	{
		sizeInBytes = rhs.sizeInBytes;
		
		if (vertexDataTypes != 0)
		{
			delete[] vertexDataTypes;
			vertexDataTypes = 0;
		}
		
		vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(&vertexDataTypes[0], &rhs.vertexDataTypes[0], sizeInBytes);

		return *this;
	}
	VertexDeclaration(const VertexDeclaration& vertexDeclaration) : vertexDataTypes(0)
	{
		sizeInBytes = vertexDeclaration.sizeInBytes;
		vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(&vertexDataTypes[0], &vertexDeclaration.vertexDataTypes[0], sizeInBytes);
	}
	VertexDeclaration(void) : vertexDataTypes(0)
	{
	}
	VertexDeclaration(const VertexDataType* vertexDataTypes, unsigned int sizeInBytes) :
		sizeInBytes(sizeInBytes), vertexDataTypes(0)
	{
		// copy the vertexdatatypes to a member pointer
		this->vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(this->vertexDataTypes, vertexDataTypes, sizeInBytes);
	};

	~VertexDeclaration(void)
	{
		if (vertexDataTypes != 0)
		{
			delete[] vertexDataTypes;
			vertexDataTypes = 0;
		}
	};

	VertexDataType* GetVertexDataTypes(unsigned int& sizeInBytes) const
	{
		sizeInBytes = this->sizeInBytes;
		return vertexDataTypes;
	};

	inline int GetSizeInBytes(void) const
	{
		return sizeInBytes;
	};

private:

	VertexDataType* vertexDataTypes;
	int sizeInBytes;
};



#endif