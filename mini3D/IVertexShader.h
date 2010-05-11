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

#ifndef AURORA_IVERTEXSHADER_H
#define AURORA_IVERTEXSHADER_H

#include <vector>

namespace mini3d
{
class IVertexShader
{
public:
	enum VertexDataType {	POSITION_FLOAT3 = 0, 
							POSITION_FLOAT4 = 1, 
							COLOR_INT = 2, 
							COLOR_FLOAT4 = 3, 
							TEXTURECOORDINATE_FLOAT2 = 4, 
							NORMAL_FLOAT3 = 5, 
							BINORMAL_FLOAT3 = 6, 
							TANGENT_FLOAT3 = 7, 
							BLENDWEIGHT_FLOAT = 8, 
							BLENDINDICES_FLOAT = 9 };
	
	// Typedefs
	typedef std::vector<int> VertexDeclarationVector;
	typedef std::vector<char> ShaderBytes;

public:
	virtual ~IVertexShader(void) {};
	virtual ShaderBytes GetVertexShader(void) = 0;
	virtual VertexDeclarationVector GetVertexDeclaration(void) = 0;
};
}

#endif
