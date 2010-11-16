
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9RENDERTARGETTEXTURE_H
#define MINI3D_D3D9RENDERTARGETTEXTURE_H

#include "../IRenderTargetTexture.h"
#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"

namespace mini3d
{
class D3D9DepthStencil;
class D3D9RenderTargetTexture : public IRenderTargetTexture, public ID3D9Texture, public ID3D9RenderTarget, public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9RenderTargetTexture(D3D9GraphicsService* graphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	~D3D9RenderTargetTexture(void);


	// ::::: IRenderTargetTexture :::::::::::::::::::::::::::::::::::::::::::::

	void SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; }
	virtual void SetSize(const int& width, const int& height) { SetRenderTargetTexture(width, height, depthTestEnabled); };

	virtual WrapStyle GetWrapStyle() const { return WRAP_CLAMP; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: ID3D9RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DSurface9* GetRenderTargetBuffer() const { return pRenderTargetSurface; };
	virtual IDirect3DSurface9* GetDepthStencil() const { return pDepthStencil; };
	virtual bool GetFullscreenCompatible() const { return true; };
	virtual bool GetWindowedCompatible() const { return true; };

	
	// ::::: ID3D9Texture ::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DTexture9* GetTextureBuffer() const { return pRenderTarget; };


	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	bool LoadRenderTarget(IDirect3DDevice9* pDevice);
	bool LoadDepthStencil(IDirect3DDevice9* pDevice);

	void UnloadRenderTarget();
	void UnloadDepthStencil();

private:

	// GraphicsDevice link
	D3D9GraphicsService* pGraphicsService;
		
	// Bitmap
	unsigned int width;
	unsigned int height;
	bool depthTestEnabled;
	bool isDirty;

	// Buffer
	IDirect3DTexture9* pRenderTarget;
	IDirect3DSurface9* pDepthStencil;
	IDirect3DSurface9* pRenderTargetSurface;
	

};
}

#endif
