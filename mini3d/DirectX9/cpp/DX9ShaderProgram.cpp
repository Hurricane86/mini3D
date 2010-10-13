
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../DX9ShaderProgram.h"
#include "../DX9GraphicsService.h"
#include <sstream>

mini3d::DX9ShaderProgram::DX9ShaderProgram(DX9GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader) :
	pGraphicsService(pGraphicsService), pVertexShader(pVertexShader), pPixelShader(pPixelShader)
{
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::DX9ShaderProgram::~DX9ShaderProgram(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::DX9ShaderProgram::LoadResource(void)
{
	isDirty = false;
}

void mini3d::DX9ShaderProgram::UnloadResource(void)
{
	isDirty = true;
}

