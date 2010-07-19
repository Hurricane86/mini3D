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

#ifndef AURORA_OGL20FULLSCREENRENDERTARGET_H
#define AURORA_OGL20FULLSCREENRENDERTARGET_H

#include "../IFullscreenRenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"
#include "OGL20GraphicsService.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20FullscreenRenderTarget : public IFullscreenRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{


public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20FullscreenRenderTarget(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~OGL20FullscreenRenderTarget();


	// ::::: IFullscreenRenderTarget ::::::::::::::::::::::::::::::::::::::::::


	virtual void SetFullscreenRenderTarget(const unsigned int& width,const  unsigned int& height,const  int& windowHandle,const  bool& depthTestEnabled,const  Quality& quality);

	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	virtual void SetSize(const int& width, const int& height);

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; };
	virtual int GetWindowHandle() const { return hWindow; };

	virtual void Display();


	// ::::: IOGL20Resource ::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods ::::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual HDC GetDeviceContext() const { return hDeviceContext; };


private:

	// ::::: Private Member Varaibles ::::::::::::::::::::::::::::::::::::::::::::
	OGL20GraphicsService* pGraphicsService;

	// Render target device context
	HDC hDeviceContext;
	GLuint pDepthStencil;
	GLuint pScreenRenderTarget;

	// Rendertarget dimensions	
	unsigned int width;
	unsigned int height;

	// window handle to render target window
	int hWindow;

	// use depth stencil
	bool depthTestEnabled;

	// quality level
	Quality quality;


	// TODO: Fix this with power of 2 comparison agains width, height...
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	
	int hBufferWindow;
	int bufferDepthTestEnabled;

	bool isDirty;

};
}

#endif