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

#ifndef AURORA_DX9PIXELSHADER_H
#define AURORA_DX9PIXELSHADER_H

#include "../IPixelShader.h"
#include <vector>

namespace mini3d
{
class DX9GraphicsService;
class DX9PixelShader : IPixelShader, IDX9Resource
{
	friend class DX9GraphicsService;

private:
	// Indices
	ShaderBytes shaderBytes_;

	// Buffer
	IDirect3DPixelShader9* pShaderBuffer_;
	bool isDirty_;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService_;

public:
	DX9PixelShader(DX9GraphicsService* graphicsService, const ShaderBytes& shaderBytes);
	~DX9PixelShader(void);

	ShaderBytes GetPixelShader() { return shaderBytes_; };

private:
	inline IDirect3DPixelShader9* GetPixelShaderBuffer(void) { return pShaderBuffer_; }

	// IDX9Resource
	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) { return isDirty_; };
};
}

#endif
