
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20ShaderProgram.h"
#include "../OGL20GraphicsService.h"
#include <GL/glext.h>
#include <GL/wglext.h>
#include <sstream>

mini3d::OGL20ShaderProgram::OGL20ShaderProgram(OGL20GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader) :
	pGraphicsService(pGraphicsService), pVertexShader(pVertexShader), pPixelShader(pPixelShader), hProgram(0)
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

	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

	PFNGLCREATEPROGRAMPROC glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	PFNGLATTACHSHADERPROC glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	PFNGLLINKPROGRAMPROC glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");

	PFNGLUSEPROGRAMPROC glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");

	hProgram = glCreateProgram();

	// Attach vertex shader
	if (pVertexShader == 0)
	{
		glAttachShader(hProgram, 0);
	}
	else
	{
		OGL20VertexShader* pOLG20VertexShader = (OGL20VertexShader*)pVertexShader;
		glAttachShader(hProgram, pOLG20VertexShader->GetVertexShaderBuffer());
	}

	// Attach pixel shader
	if (pPixelShader == 0)
	{
		glAttachShader(hProgram, 0);
	}
	else
	{
		OGL20PixelShader* pOLG20PixelShader = (OGL20PixelShader*)pPixelShader;
		glAttachShader(hProgram, pOLG20PixelShader->GetPixelShaderBuffer());
	}

	glLinkProgram(hProgram);
	printLog(hProgram);
	
	// Get attrib information

	PFNGLGETPROGRAMIVPROC glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");

	char *name = new char[1024];
	GLint active_attribs, max_length;

	GLint size;
	GLenum type;

	glGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	glGetProgramiv(hProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		glGetActiveAttrib(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << glGetAttribLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		OutputDebugStringA(s.c_str());
	}


	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");

	glGetProgramiv(hProgram, GL_ACTIVE_UNIFORMS, &active_attribs);
	glGetProgramiv(hProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

	for (int i = 0; i < active_attribs; i++) 
	{
		glGetActiveUniform(hProgram, i, max_length + 1, NULL, &size, &type, name);
		std::stringstream ss;
		ss << "Type: " << type << " Name: " << name << " AttribLocation: " << glGetUniformLocation(hProgram, name) << "\n";
		std::string s = ss.str();
		OutputDebugStringA(s.c_str());
	}

	delete[] name;
	// END Attrib information

	isDirty = false;
}

void mini3d::OGL20ShaderProgram::UnloadResource(void)
{
	if (hProgram != 0)
	{

		PFNGLDELETEPROGRAMPROC glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");

		// if this is the currently loaded pixel shader, release it
		if (pGraphicsService->GetShaderProgram() == this)
			pGraphicsService->SetShaderProgram(0);

		glDeleteProgram(hProgram);
		hProgram  = 0;
	}

	isDirty = true;
}

void mini3d::OGL20ShaderProgram::printLog(GLuint obj)
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