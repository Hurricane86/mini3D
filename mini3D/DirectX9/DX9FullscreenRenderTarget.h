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

#ifndef AURORA_DX9FULLSCREENRENDERTARGET_H
#define AURORA_DX9FULLSCREENRENDERTARGET_H

#include "../IFullscreenRenderTarget.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9RenderTarget.h"
#include "internal/IDX9Resource.h"

// Todo: make a c-file with implementations of all this

namespace mini3d
{
class DX9FullscreenRenderTarget : public IFullscreenRenderTarget, public IDX9RenderTarget, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9FullscreenRenderTarget(DX9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) : pGraphicsService(pGraphicsService), width(width), height(height), quality(quality), depthTestEnabled(depthTestEnabled), hWindow(hWindow) { };
	~DX9FullscreenRenderTarget(void){};


	// ::::: IFullscreenRenderTarget ::::::::::::::::::::::::::::::::::::::::::

	virtual void SetFullScreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) { };
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	virtual void SetSize(const int& width, const int& height) { this->width = width; this->height = height; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; };
	virtual int GetWindowHandle() const { return hWindow; };

	virtual void Display();


	// ::::: IDX9Resource ::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource() { return; };
	virtual void UnloadResource(){ return; };
	virtual bool GetIsDirty() const { return false; };


	// ::::: IDX9RenderTarget ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DSurface9* GetRenderTargetBuffer(void) const { return 0; };

	virtual bool GetFullscreenCompatible(void) const { return true; };
	virtual bool GetWindowedCompatible(void) const { return false; };
	

	// ::::: Public Methods ::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDepthStencil* GetDepthStencil(void) const { return 0; };


private:

	// ::::: Private Member Varaibles ::::::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	// Rendertarget dimensions	
	int width;
	int height;

	// window handle to render target window
	int hWindow;

	// use depth stencil
	bool depthTestEnabled;

	// quality level
	Quality quality;

};
}
#endif