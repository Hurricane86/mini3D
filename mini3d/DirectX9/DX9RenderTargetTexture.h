
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9RENDERTARGETTEXTURE_H
#define MINI3D_DX9RENDERTARGETTEXTURE_H

#include "../IRenderTargetTexture.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9Resource.h"

namespace mini3d
{
class DX9DepthStencil;
class DX9RenderTargetTexture : public IRenderTargetTexture, public IDX9Texture, public IDX9RenderTarget, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9RenderTargetTexture(DX9GraphicsService* graphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	~DX9RenderTargetTexture(void);


	// ::::: IRenderTargetTexture :::::::::::::::::::::::::::::::::::::::::::::

	void SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; }
	virtual void SetSize(const int& width, const int& height) { SetRenderTargetTexture(width, height, depthTestEnabled); };

	virtual WrapStyle GetWrapStyle() const { return WRAP_CLAMP; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: IDX9RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DSurface9* GetRenderTargetBuffer() const { return pRenderTargetSurface; };
	virtual IDepthStencil* GetDepthStencil() const { return pDepthStencil; };
	virtual bool GetFullscreenCompatible() const { return true; };
	virtual bool GetWindowedCompatible() const { return true; };

	
	// ::::: IDX9Texture ::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DTexture9* GetTextureBuffer() const { return pRenderTarget; };


private:

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;
		
	// Bitmap
	unsigned int width;
	unsigned int height;
	bool depthTestEnabled;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	IDirect3DTexture9* pRenderTarget;
	IDepthStencil* pDepthStencil;
	IDirect3DSurface9* pRenderTargetSurface;
	

};
}

#endif
