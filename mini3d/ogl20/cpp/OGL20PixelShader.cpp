
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20PixelShader.h"
#include "../OGL20GraphicsService.h"
#include <cstdlib>
#include <cstring>

mini3d::OGL20PixelShader::OGL20PixelShader(OGL20GraphicsService* pGraphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes) :
	pGraphicsService(pGraphicsService), pShaderBuffer(0), sizeInBytes(sizeInBytes), pOS(pGraphicsService->GetOS())
{
	this->pShaderBytes = malloc(sizeInBytes);
	std::memcpy(this->pShaderBytes, pShaderBytes, sizeInBytes);

	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20PixelShader::~OGL20PixelShader(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20PixelShader::LoadResource(void)
{
	
	// If the buffer exists tear it down.
	if (pShaderBuffer != 0)
	{
		UnloadResource();
	}
	
	pShaderBuffer = pOS->GLCreateShader(GL_FRAGMENT_SHADER);
	pOS->GLShaderSource(pShaderBuffer, 1, (const GLchar**)&pShaderBytes, (GLint*)&sizeInBytes);
	pOS->GLCompileShader(pShaderBuffer);

	printLog(pShaderBuffer);
	isDirty = false;
} 

void mini3d::OGL20PixelShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		// if this is the currently loaded pixel shader, release it
		//if (pGraphicsService_->GetPixelShader() == this)
		//	pGraphicsService_->SetPixelShader(0);

		pOS->GLDeleteShader(pShaderBuffer);
		pShaderBuffer = 0;
	}

	isDirty = true;
}

void mini3d::OGL20PixelShader::printLog(GLuint obj)
{
	int maxLength;
	pOS->GLGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	int infologLength;
	char* infoLog = new char[maxLength];
	pOS->GLGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
	{
		pOS->Log((char*)"\nDEBUG INFO ---------\n");
		pOS->Log(infoLog);
	}

	delete [] infoLog;
}