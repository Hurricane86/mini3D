
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_DX9SHADERPROGRAM_H
#define AURORA_DX9SHADERPROGRAM_H

#include "../IPixelShader.h"
#include "../IVertexShader.h"
#include "../IShaderProgram.h"
#include "internal/IDX9Resource.h"


namespace mini3d
{
class DX9GraphicsService;
class DX9ShaderProgram : public IShaderProgram, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9ShaderProgram(DX9GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~DX9ShaderProgram();


	// ::::: IShaderProgram :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IPixelShader* GetPixelShader() const { return pPixelShader; };
	virtual IVertexShader* GetVertexShader() const { return pVertexShader; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; };


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// Indices
	IPixelShader* pPixelShader;
	IVertexShader* pVertexShader;

	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

};
}

#endif
