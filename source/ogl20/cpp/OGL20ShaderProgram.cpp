
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20ShaderProgram.h"
#include "../OGL20GraphicsService.h"
#include <sstream>

mini3d::OGL20ShaderProgram::OGL20ShaderProgram(OGL20GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader) :
	pGraphicsService(pGraphicsService), pVertexShader(pVertexShader), pPixelShader(pPixelShader), hProgram(0), pOGLWrapper(pGraphicsService->GetOGLWrapper()), pOSWrapper(pGraphicsService->GetOSWrapper())
{
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20ShaderProgram::~OGL20ShaderProgram(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20ShaderProgram::LoadResource(void)
{

	// If the program exists tear it down.
	if (hProgram != 0)
	{
		UnloadResource();
	}

	hProgram = pOGLWrapper->GLCreateProgram();

	// Attach vertex shader
	if (pVertexShader == 0)
	{
		pOGLWrapper->GLAttachShader(hProgram, 0);
	}
	else
	{
		OGL20VertexShader* pOLG20VertexShader = (OGL20VertexShader*)pVertexShader;
		pOGLWrapper->GLAttachShader(hProgram, pOLG20VertexShader->GetVertexShaderBuffer());
	}

	// Attach pixel shader
	if (pPixelShader == 0)
	{
		pOGLWrapper->GLAttachShader(hProgram, 0);
	}
	else
	{
		OGL20PixelShader* pOLG20PixelShader = (OGL20PixelShader*)pPixelShader;
		pOGLWrapper->GLAttachShader(hProgram, pOLG20PixelShader->GetPixelShaderBuffer());
	}

	pOGLWrapper->GLLinkProgram(hProgram);
	printLog(hProgram);
	
	// Get attrib information

	char *name = new char[1024];
	GLint active_attribs, max_length;

	GLint size;
	GLenum type;

	pOGLWrapper->GLGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	pOGLWrapper->GLGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		pOGLWrapper->GLGetActiveAttrib(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << pOGLWrapper->GLGetAttribLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		pOSWrapper->Log((char*)s.c_str());
	}

	pOGLWrapper->GLGetProgramiv(hProgram, GL_ACTIVE_UNIFORMS, &active_attribs);
	pOGLWrapper->GLGetProgramiv(hProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		pOGLWrapper->GLGetActiveUniform(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << pOGLWrapper->GLGetUniformLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		pOSWrapper->Log((char*)s.c_str());
	}

	delete[] name;
	// END Attrib information

	isDirty = false;
}

void mini3d::OGL20ShaderProgram::UnloadResource(void)
{
	if (hProgram != 0)
	{
		// if this is the currently loaded pixel shader, release it
		if (pGraphicsService->GetShaderProgram() == this)
			pGraphicsService->SetShaderProgram(0);

		pOGLWrapper->GLDeleteProgram(hProgram);
		hProgram  = 0;
	}

	isDirty = true;
}

void mini3d::OGL20ShaderProgram::printLog(GLuint obj)
{
	int maxLength;
	pOGLWrapper->GLGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	int infologLength = 0;
	char* infoLog = new char[maxLength];
	pOGLWrapper->GLGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
 
	if (infologLength > 0)
	{
		pOSWrapper->Log((char*)"\nDEBUG INFO ---------\n");
		pOSWrapper->Log(infoLog);
	}

	delete [] infoLog;
}