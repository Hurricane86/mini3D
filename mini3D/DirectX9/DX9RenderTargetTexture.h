/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef AURORA_DX9RENDERTARGETTEXTURE_H
#define AURORA_DX9RENDERTARGETTEXTURE_H

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
