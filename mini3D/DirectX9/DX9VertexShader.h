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

private:
	// Indices
	ShaderBytes shaderBytes_;
	VertexDeclarationVector vertexDeclaration_;
	
	// Buffer
	IDirect3DVertexShader9* pShaderBuffer_;
	bool isDirty_;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService_;

public:
	DX9VertexShader(DX9GraphicsService* pGraphicsService, const ShaderBytes& shaderBytes, const VertexDeclarationVector& vertexDeclaration);
	~DX9VertexShader(void);

	virtual ShaderBytes GetVertexShader(void) { return shaderBytes_; };
	virtual VertexDeclarationVector GetVertexDeclaration(void) { return vertexDeclaration_; };

private:
	virtual IDirect3DVertexShader9* GetVertexShaderBuffer(void) { return pShaderBuffer_; };

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void) { return isDirty_; };
};
}

#endif
