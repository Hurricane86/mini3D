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
class OGL20ShaderProgram : public IShaderProgram, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20ShaderProgram(OGL20GraphicsService* graphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~OGL20ShaderProgram();


	// ::::: IShaderProgram :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IPixelShader* GetPixelShader() const { return pPixelShader; };
	virtual IVertexShader* GetVertexShader() const { return pVertexShader; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; };


	// ::::: Public Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	inline GLuint GetProgram() { return hProgram; }
	void printLog(GLuint obj);


private:
	// Indices
	IPixelShader* pPixelShader;
	IVertexShader* pVertexShader;

	GLuint hProgram;

	bool isDirty;

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;


};
}

#endif
