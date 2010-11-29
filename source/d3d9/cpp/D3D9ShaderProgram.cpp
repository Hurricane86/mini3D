
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "../D3D9ShaderProgram.h"
#include "../D3D9GraphicsService.h"
#include <sstream>

mini3d::D3D9ShaderProgram::D3D9ShaderProgram(D3D9GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader) :
	pGraphicsService(pGraphicsService), pVertexShader(pVertexShader), pPixelShader(pPixelShader)
{
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::D3D9ShaderProgram::~D3D9ShaderProgram(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::D3D9ShaderProgram::LoadResource(void)
{
	isDirty = false;
}

void mini3d::D3D9ShaderProgram::UnloadResource(void)
{
	isDirty = true;
}

#endif