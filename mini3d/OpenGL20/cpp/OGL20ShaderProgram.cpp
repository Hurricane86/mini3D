
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20ShaderProgram.h"
#include "../OGL20GraphicsService.h"
#include <GL/glext.h>
#include <GL/wglext.h>
#include <sstream>

mini3d::OGL20ShaderProgram::OGL20ShaderProgram(OGL20GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader) :
	pGraphicsService(pGraphicsService), pVertexShader(pVertexShader), pPixelShader(pPixelShader), hProgram(0), pOS(pGraphicsService->GetOS())
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

	hProgram = pOS->GLCreateProgram();

	// Attach vertex shader
	if (pVertexShader == 0)
	{
		pOS->GLAttachShader(hProgram, 0);
	}
	else
	{
		OGL20VertexShader* pOLG20VertexShader = (OGL20VertexShader*)pVertexShader;
		pOS->GLAttachShader(hProgram, pOLG20VertexShader->GetVertexShaderBuffer());
	}

	// Attach pixel shader
	if (pPixelShader == 0)
	{
		pOS->GLAttachShader(hProgram, 0);
	}
	else
	{
		OGL20PixelShader* pOLG20PixelShader = (OGL20PixelShader*)pPixelShader;
		pOS->GLAttachShader(hProgram, pOLG20PixelShader->GetPixelShaderBuffer());
	}

	pOS->GLLinkProgram(hProgram);
	printLog(hProgram);
	
	// Get attrib information

	char *name = new char[1024];
	GLint active_attribs, max_length;

	GLint size;
	GLenum type;

	pOS->GLGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	pOS->GLGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		pOS->GLGetActiveAttrib(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << pOS->GLGetAttribLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		pOS->Log((char*)s.c_str());
	}

	pOS->GLGetProgramiv(hProgram, GL_ACTIVE_UNIFORMS, &active_attribs);
	pOS->GLGetProgramiv(hProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		pOS->GLGetActiveUniform(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << pOS->GLGetUniformLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		pOS->Log((char*)s.c_str());
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

		pOS->GLDeleteProgram(hProgram);
		hProgram  = 0;
	}

	isDirty = true;
}

void mini3d::OGL20ShaderProgram::printLog(GLuint obj)
{
	int maxLength;
	pOS->GLGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	int infologLength = 0;
	char* infoLog = new char[maxLength];
	pOS->GLGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
 
	if (infologLength > 0)
	{
		pOS->Log("\nDEBUG INFO ---------\n");
		pOS->Log(infoLog);
	}

	delete [] infoLog;
}