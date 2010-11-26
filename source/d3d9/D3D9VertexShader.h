
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_D3D9VERTEXSHADER_H
#define MINI3D_D3D9VERTEXSHADER_H

#include "../mini3d/IVertexShader.h"
#include "D3D9GraphicsService.h"

namespace mini3d
{
class D3D9VertexShader : IVertexShader, ID3D9Resource
{
friend class D3D9GraphicsService;


public:

	
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9VertexShader(D3D9GraphicsService* pGraphicsService, const void* shaderBytes, const unsigned int& sizeInBytes, const VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	~D3D9VertexShader(void);

	virtual void* GetVertexShader(unsigned int& sizeInBytes) const { sizeInBytes = this->sizeInBytes; return pShaderBytes; };
	virtual VertexDataType* GetVertexDeclaration(unsigned int& vertexDataCount) const;

	
	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DVertexShader9* GetVertexShaderBuffer(void) { return pShaderBuffer; };
	

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// Indices
	void* pShaderBytes;
	unsigned int sizeInBytes;
		
	VertexDataType* vertexDeclaration;
	unsigned int vertexDataCount;
	
	// Buffer
	IDirect3DVertexShader9* pShaderBuffer;
	bool isDirty;

	// GraphicsDevice link
	D3D9GraphicsService* pGraphicsService;

	};
}

#endif
#endif
