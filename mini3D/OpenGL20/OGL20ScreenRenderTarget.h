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

#ifndef AURORA_IOGL20SCREENRENDERTARGET_H
#define AURORA_IOGL20SCREENRENDERTARGET_H

#include "../IScreenRenderTarget.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20DepthStencil;
class OGL20ScreenRenderTarget : public IScreenRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{
// TODO: Public and private

private:
	OGL20GraphicsService* pGraphicsService;
	GLuint pScreenRenderTarget;
	OGL20DepthStencil* pDepthStencil;

	int width;
	int height;
	int hWindow;
	HDC hDeviceContext;
	bool depthTestEnabled;
	Quality quality;

	// TODO: Fix this with power of 2 comparison agains width, height...
	int bufferWidth;
	int bufferHeight;
	int hBufferWindow;
	int bufferDepthTestEnabled;
	
	bool isDirty;

private:
	virtual int GetType() { return 2; }

public:
	OGL20ScreenRenderTarget(OGL20GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality);
	virtual ~OGL20ScreenRenderTarget(void);

	virtual void SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality);
	HDC GetRenderTargetBuffer(void);

	void LoadResource(void);
	void UnloadResource(void);

	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);
	virtual bool GetDepthTestEnabled(void);
	virtual void Display(void);
	bool GetIsDirty(void);
};
}

#endif
