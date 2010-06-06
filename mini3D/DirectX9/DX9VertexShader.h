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

#ifndef AURORA_DX9VERTEXSHADER_H
#define AURORA_DX9VERTEXSHADER_H

#include "../IVertexShader.h"
#include "DX9GraphicsService.h"
#include <vector>

namespace mini3d
{
class DX9VertexShader : IVertexShader, IDX9Resource
{
friend class DX9GraphicsService;


public:

	
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9VertexShader(DX9GraphicsService* pGraphicsService, const void* shaderBytes, const unsigned int& sizeInBytes, const VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	~DX9VertexShader(void);

	virtual void* GetVertexShader(unsigned int& sizeInBytes) const { sizeInBytes = this->sizeInBytes; return pShaderBytes; };
	virtual VertexDataType* GetVertexDeclaration(unsigned int& vertexDataCount) const;

	
	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DVertexShader9* GetVertexShaderBuffer(void) { return pShaderBuffer; };
	

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// Indices
	void* pShaderBytes;
	unsigned int sizeInBytes;
		
	VertexDataType* vertexDeclaration;
	unsigned int vertexDataCount;
	
	// Buffer
	IDirect3DVertexShader9* pShaderBuffer;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	};
}

#endif
