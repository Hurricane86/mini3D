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

#ifndef AURORA_IOGL20WINDOWRENDERTARGET_H
#define AURORA_IOGL20WINDOWRENDERTARGET_H

#include "../IWindowRenderTarget.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20DepthStencil;
class OGL20WindowRenderTarget : public IWindowRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20WindowRenderTarget(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~OGL20WindowRenderTarget(void);


	// ::::: IWindowRenderTarget ::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetWindowRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	virtual void SetSize(const int& width, const int& height);
	
	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; };
	virtual int GetWindowHandle() const { return hWindow; };

	virtual void Display();


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }

	
	// ::::: IOGL20RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::

	HDC  GetRenderTargetBuffer(void) const { return hDeviceContext; }
	GLuint GetDepthStencil(void) const { return pDepthStencil; }

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	OGL20GraphicsService* pGraphicsService;

	HDC hDeviceContext;
	GLuint pDepthStencil;

	unsigned int width;
	unsigned int height;
	int hWindow;

	bool depthTestEnabled;
	Quality quality;

	// TODO: Fix this with power of 2 comparison agains width, height...
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	
	int hBufferWindow;
	int bufferDepthTestEnabled;

	GLuint pScreenRenderTarget;

	bool isDirty;

};
}

#endif
