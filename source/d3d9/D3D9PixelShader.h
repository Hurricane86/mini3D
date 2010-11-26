
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_D3D9PIXELSHADER_H
#define MINI3D_D3D9PIXELSHADER_H

#include "../mini3d/IPixelShader.h"
#include <vector>

namespace mini3d
{
class D3D9GraphicsService;
class D3D9PixelShader : public IPixelShader, public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9PixelShader(D3D9GraphicsService* graphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes);
	~D3D9PixelShader(void);


	// ::::: IPixelShader :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetPixelShader(unsigned int& sizeInBytes) { sizeInBytes = this->sizeInBytes; return pShaderBytes; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	inline IDirect3DPixelShader9* GetPixelShaderBuffer(void) { return pShaderBuffer; }


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	D3D9GraphicsService* pGraphicsService;

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
#endif
