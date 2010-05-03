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

#include "../OGL20PixelShader.h"
#include "../OGL20GraphicsService.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20PixelShader::OGL20PixelShader(OGL20GraphicsService* pGraphicsService, const ShaderBytes& shaderBytes) :
	pGraphicsService_(pGraphicsService), shaderBytes_(shaderBytes), pShaderBuffer_(0)
{
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20PixelShader::~OGL20PixelShader(void)
{
	UnloadResource();
	pGraphicsService_->RemoveResource(this);
}

void mini3d::OGL20PixelShader::LoadResource(void)
{
	
	PFNGLCREATESHADERPROC glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	PFNGLSHADERSOURCEPROC glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	PFNGLCOMPILESHADERPROC glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");

	// If the buffer exists tear it down.
	if (pShaderBuffer_ != 0)
	{
		UnloadResource();
	}
	
	pShaderBuffer_ = glCreateShader(GL_FRAGMENT_SHADER);

	std::vector<char> shaderBytes(shaderBytes_);
	shaderBytes.push_back(0); // add a null termination character
	const GLchar* pShaderBytes = &(shaderBytes[0]);

	glShaderSource(pShaderBuffer_, 1, &pShaderBytes, NULL);
	glCompileShader(pShaderBuffer_);

	printLog(pShaderBuffer_);
	isDirty_ = false;
}

void mini3d::OGL20PixelShader::UnloadResource(void)
{
	if (pShaderBuffer_ != 0)
	{

		PFNGLDELETESHADERPROC glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

		// if this is the currently loaded pixel shader, release it
		//if (pGraphicsService_->GetPixelShader() == this)
		//	pGraphicsService_->SetPixelShader(0);

		glDeleteShader(pShaderBuffer_);
		pShaderBuffer_ = 0;
	}

	isDirty_ = true;
}

void mini3d::OGL20PixelShader::printLog(GLuint obj)
{
	PFNGLISSHADERPROC glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");

	PFNGLGETSHADERIVPROC glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	PFNGLGETPROGRAMIVNVPROC glGetProgramiv = (PFNGLGETPROGRAMIVNVPROC)wglGetProcAddress("glGetProgramiv");
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	
	int infologLength = 0;
	int maxLength;
	
	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
			
	char* infoLog = new char[maxLength];
	LPCWSTR* infoLogW = new LPCWSTR[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
 
	OutputDebugStringA("\nDEBUG INFO ---------\n");

	if (infologLength > 0)
		OutputDebugStringA(infoLog);

	delete [] infoLog;
}