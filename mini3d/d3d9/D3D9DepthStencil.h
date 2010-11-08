
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9DEPTHSTENCIL_H
#define MINI3D_D3D9DEPTHSTENCIL_H

#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"

namespace mini3d
{
class D3D9DepthStencil : public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9DepthStencil(D3D9GraphicsService* graphicsService, const unsigned int& width, const unsigned int& height);
	~D3D9DepthStencil(void);

	// ::::: IDepthStencil ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetDepthStencil(const unsigned int& width, const unsigned int& height);
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };

	
	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DSurface9* GetDepthStencilBuffer(void) { return pDepthStencil; };


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	D3D9GraphicsService* pGraphicsService;

	// Stencil dimentions
	unsigned int width;
	unsigned int height;

	// Direct3D9 Stencil Surface
	IDirect3DSurface9* pDepthStencil;
	
	// Size of the currently loaded buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;

	// Keps track of the state of the resource
	bool isDirty;


};
}

#endif
