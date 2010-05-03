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

#ifndef AURORA_OGL20SHADERPROGRAM_H
#define AURORA_OGL20SHADERPROGRAM_H

#include "../IPixelShader.h"
#include "../IVertexShader.h"
#include "../IShaderProgram.h"
#include "internal/IOGL20Resource.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20GraphicsService;
class OGL20ShaderProgram : IShaderProgram, IOGL20Resource
{
	friend class OGL20GraphicsService;

private:
	// Indices
	IPixelShader* pPixelShader;
	IVertexShader* pVertexShader;

	GLuint hProgram;

	bool isDirty_;

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService_;

public:
	OGL20ShaderProgram(OGL20GraphicsService* graphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~OGL20ShaderProgram(void);

private:
	inline GLuint GetProgram(void) { return hProgram; }
	void printLog(GLuint obj);

	// IOGL20Resource
	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) { return isDirty_; };
};
}

#endif
