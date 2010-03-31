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

#ifndef AURORA_IVERTEXBUFFER_H
#define AURORA_IVERTEXBUFFER_H

#include <vector>

namespace mini3d
{
class IVertexBuffer
{
public:
	enum VertexDataType { POSITION = 0, COLOR = 1, TEXTURECOORDINATE = 2, NORMAL = 3, BINORMAL = 4, TANGENT = 5, BLENDWEIGHT = 6, BLENDINDICES = 7 };
	
	// Typedefs
	typedef std::vector<int> VertexDeclarationVector;

	virtual ~IVertexBuffer(void) {};

	virtual void SetVertices(void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration) = 0;
	virtual void* GetVertices(unsigned int& count, VertexDeclarationVector& vertexDeclaration) = 0;

	virtual unsigned int GetVertexCount() = 0;
	virtual VertexDeclarationVector GetVertexDeclaration() = 0;
};
}

#endif
