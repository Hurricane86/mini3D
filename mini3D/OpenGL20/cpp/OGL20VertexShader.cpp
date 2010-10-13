
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


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

}

void mini3d::OGL20VertexShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		PFNGLDELETESHADERPROC glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

		glDeleteShader(pShaderBuffer);
		pShaderBuffer = 0;
	}

	isDirty = true;
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
		case IVertexShader::POSITION_FLOAT3:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 3;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 12;
			break;				
		case IVertexShader::POSITION_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::COLOR_INT:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 1;
			vertexAttributes[i].type = GL_INT;
			offset += 4;
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
		case IVertexShader::NORMAL_FLOAT3:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 3;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 12;
			break;				
		case IVertexShader::NORMAL_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::BINORMAL_FLOAT3:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 3;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 12;
			break;
		case IVertexShader::BINORMAL_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::TANGENT_FLOAT3:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 3;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 12;
			break;
		case IVertexShader::TANGENT_FLOAT4:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 4;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 16;
			break;
		case IVertexShader::BLENDWEIGHT_FLOAT:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 1;
			vertexAttributes[i].type = GL_FLOAT;
			offset += 1;
			break;
		case IVertexShader::BLENDINDICES_INT:
			vertexAttributes[i].normalized = GL_FALSE;
			vertexAttributes[i].size = 1;
			vertexAttributes[i].type = GL_INT;
			offset += 1;
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