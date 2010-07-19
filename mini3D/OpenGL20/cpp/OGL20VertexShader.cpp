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

#include "../OGL20VertexShader.h"
#include <GL/glext.h>
#include <GL/wglext.h>
#include <iostream>
#include <iomanip>
#include <sstream>


mini3d::OGL20VertexShader::OGL20VertexShader(OGL20GraphicsService* pGraphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount) :
	pGraphicsService(pGraphicsService), pShaderBuffer(0), vertexAttributes(0)
{
	// Vertex shader data
	this->sizeInBytes = sizeInBytes;

	this->pShaderBytes = malloc(sizeInBytes);
	memcpy(this->pShaderBytes, pShaderBytes, sizeInBytes);

	// VertexDeclaration
	this->vertexDataCount = vertexDataCount;

	this->vertexDeclaration = new VertexDataType[vertexDataCount];
	memcpy(this->vertexDeclaration, vertexDeclaration, vertexDataCount * sizeof(VertexDataType));

	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::IVertexShader::VertexDataType* mini3d::OGL20VertexShader::GetVertexDeclaration(unsigned int& vertexDataCount) const
{ 
	vertexDataCount = this->vertexDataCount;
	unsigned int sizeInBytes = this->vertexDataCount * sizeof(VertexDataType);

	VertexDataType* pVertexDeclarationCopy = new IVertexShader::VertexDataType[sizeInBytes]; 
	memcpy(pVertexDeclarationCopy, vertexDeclaration, sizeInBytes);
	
	return pVertexDeclarationCopy;
}

mini3d::OGL20VertexShader::~OGL20VertexShader(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);

	free(pShaderBytes);
	delete[] vertexDeclaration;
}

void mini3d::OGL20VertexShader::LoadResource(void)
{
	PFNGLCREATESHADERPROC glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	PFNGLSHADERSOURCEPROC glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	PFNGLCOMPILESHADERPROC glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");

	// If the buffer exists tear it down.
	if (pShaderBuffer != 0)
	{
		UnloadResource();
	}

	pShaderBuffer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(pShaderBuffer, 1, (const GLchar**)&pShaderBytes, (const GLint*)&sizeInBytes);
	glCompileShader(pShaderBuffer);

	printLog(pShaderBuffer);
	isDirty = false;

	// update the vertex Attributes
	CreateOGL20VertexAttributes();

	// TODO: What about this one and the one below?
	// load the vertex declaration into the pool
	//pGraphicsService->PoolVertexDeclaration(vertexDeclaration);
}

void mini3d::OGL20VertexShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		PFNGLDELETESHADERPROC glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

		// if this is the currently loaded pixel shader, release it
		//if (pGraphicsService_->GetVertexShader() == this)
		//	pGraphicsService_->SetVertexShader(0);

		glDeleteShader(pShaderBuffer);
		pShaderBuffer = 0;
	}

	isDirty = true;

	// TODO: What about this one and the one above?
	// remove the vertex declaration from the pool
	// pGraphicsService_->ReleaseVertexDeclaration(vertexDeclaration_);

}

void mini3d::OGL20VertexShader::CreateOGL20VertexAttributes()
{
	if (vertexAttributes != 0)
		delete[] vertexAttributes;

	vertexAttributes = new OGL20VertexAttribute[vertexDataCount];

	// cumulative offset for the vertexelements
	int offset = 0;
	int textureUsageIndex = 0;
	int uvStream = 0;
	int positionUsageIndex = 0;
	int colorUsageIndex = 0;


	for (unsigned int i = 0; i < vertexDataCount; i++)
	{
		// these are the same for all
		vertexAttributes[i].index = i;
		vertexAttributes[i].pointer = (GLvoid*)offset;
		
		// set the specific parameters
		switch (vertexDeclaration[i])
		{
				
		case IVertexShader::POSITION_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::COLOR_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::TEXTURECOORDINATE_FLOAT2:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 2;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 8;
			break;
		}
	}

	for (unsigned int i = 0; i < vertexDataCount; i++)
		vertexAttributes[i].stride = offset;
}


void mini3d::OGL20VertexShader::printLog(GLuint obj)
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