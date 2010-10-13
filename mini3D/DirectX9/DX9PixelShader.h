
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9PIXELSHADER_H
#define MINI3D_DX9PIXELSHADER_H

#include "../IPixelShader.h"
#include <vector>

namespace mini3d
{
class DX9GraphicsService;
class DX9PixelShader : public IPixelShader, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9PixelShader(DX9GraphicsService* graphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes);
	~DX9PixelShader(void);


	// ::::: IPixelShader :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetPixelShader(unsigned int& sizeInBytes) { sizeInBytes = this->sizeInBytes; return pShaderBytes; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	inline IDirect3DPixelShader9* GetPixelShaderBuffer(void) { return pShaderBuffer; }


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	// Indices
	void* pShaderBytes;

	// Number of bytes in shaderBytes array
	unsigned int sizeInBytes;

	// Buffer
	IDirect3DPixelShader9* pShaderBuffer;

	// Keps track of the state of the resource
	bool isDirty;


};
}

#endif
