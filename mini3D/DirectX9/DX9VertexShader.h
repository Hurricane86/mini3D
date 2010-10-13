
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_DX9VERTEXSHADER_H
#define AURORA_DX9VERTEXSHADER_H

#include "../IVertexShader.h"
#include "DX9GraphicsService.h"

namespace mini3d
{
class DX9VertexShader : IVertexShader, IDX9Resource
{
friend class DX9GraphicsService;


public:

	
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9VertexShader(DX9GraphicsService* pGraphicsService, const void* shaderBytes, const unsigned int& sizeInBytes, const VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	~DX9VertexShader(void);

	virtual void* GetVertexShader(unsigned int& sizeInBytes) const { sizeInBytes = this->sizeInBytes; return pShaderBytes; };
	virtual VertexDataType* GetVertexDeclaration(unsigned int& vertexDataCount) const;

	
	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

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
	DX9GraphicsService* pGraphicsService;

	};
}

#endif
