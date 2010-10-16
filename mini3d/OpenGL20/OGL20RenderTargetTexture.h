
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20RENDERTARGETTEXTURE_H
#define MINI3D_OGL20RENDERTARGETTEXTURE_H

#include "../IRenderTargetTexture.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20RenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20Texture.h"
#include "os/OSFunctionSelector.h"

namespace mini3d
{
class OGL20RenderTargetTexture : public IRenderTargetTexture, public IOGL20Texture, public IOGL20RenderTarget, public IOGL20Resource
{
friend class OGL20GraphicsService;


public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20RenderTargetTexture(OGL20GraphicsService* graphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	~OGL20RenderTargetTexture(void);


	// ::::: IRenderTargetTexture :::::::::::::::::::::::::::::::::::::::::::::

	void SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);

	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; }
	virtual void SetSize(const int& width, const int& height) { SetRenderTargetTexture(width, height, depthTestEnabled); };

	virtual WrapStyle GetWrapStyle() const { return WRAP_CLAMP; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: IOGL20RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::

	virtual GLuint GetRenderTargetBuffer() const { return pRenderTarget; };
	virtual GLuint GetDepthStencil() const { return pDepthStencil; };
	virtual bool GetFullscreenCompatible() const { return true; };
	virtual bool GetWindowedCompatible() const { return true; };

	
	// ::::: IOGL20Texture ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual GLuint GetTextureBuffer() const { return pTexture; };


private:

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;
	OSFunctions* pOS;

	// Bitmap
	unsigned int width;
	unsigned int height;
	bool depthTestEnabled;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	GLuint pTexture;
	GLuint pRenderTarget;
	GLuint pDepthStencil;


};
}

#endif
