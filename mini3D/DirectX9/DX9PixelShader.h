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
class DX9PixelShader : public IPixelShader, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9PixelShader(DX9GraphicsService* graphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes);
	~DX9PixelShader(void);


	// ::::: IPixelShader :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetPixelShader(unsigned int& sizeInBytes) { sizeInBytes = this->sizeInBytes; return pShaderBytes; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	inline IDirect3DPixelShader9* GetPixelShaderBuffer(void) { return pShaderBuffer; }


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	// Indices
	void* pShaderBytes;

	// Number of bytes in shaderBytes array
	unsigned int sizeInBytes;

	// Buffer
	IDirect3DPixelShader9* pShaderBuffer;

	// Keps track of the state of the resource
	bool isDirty;


};
}

#endif
