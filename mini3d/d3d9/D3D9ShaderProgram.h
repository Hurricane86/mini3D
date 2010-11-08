
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9SHADERPROGRAM_H
#define MINI3D_D3D9SHADERPROGRAM_H

#include "../IPixelShader.h"
#include "../IVertexShader.h"
#include "../IShaderProgram.h"
#include "internal/ID3D9Resource.h"


namespace mini3d
{
class D3D9GraphicsService;
class D3D9ShaderProgram : public IShaderProgram, public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9ShaderProgram(D3D9GraphicsService* pGraphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~D3D9ShaderProgram();


	// ::::: IShaderProgram :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IPixelShader* GetPixelShader() const { return pPixelShader; };
	virtual IVertexShader* GetVertexShader() const { return pVertexShader; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

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
	D3D9GraphicsService* pGraphicsService;

};
}

#endif
