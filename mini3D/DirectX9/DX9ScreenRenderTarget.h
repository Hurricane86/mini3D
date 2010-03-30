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

#ifndef AURORA_IDX9SCREENRENDERTARGET_H
#define AURORA_IDX9SCREENRENDERTARGET_H

#include "../IScreenRenderTarget.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9DepthStencil;

class DX9ScreenRenderTarget : public IScreenRenderTarget, public IDX9RenderTarget, public IDX9Resource
{
// TODO: Public and private

private:
	DX9GraphicsService* pGraphicsService;
	IDirect3DSwapChain9* pScreenRenderTarget;
	DX9DepthStencil* pDepthStencil;

	int width;
	int height;
	int hWindow;
	int depthTestEnabled;

	// TODO: Fix this with power of 2 comparison agains width, height...
	int bufferWidth;
	int bufferHeight;
	int hBufferWindow;
	int bufferDepthTestEnabled;
	
	bool isDirty;

public:
	DX9ScreenRenderTarget(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled);
	virtual ~DX9ScreenRenderTarget(void);

	virtual void SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled);
	IDirect3DSurface9*  GetRenderTargetBuffer(void);

	void LoadResource(void);
	void UnloadResource(void);

	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);
	virtual bool GetDepthTestEnabled(void);
	virtual IDepthStencil* GetDepthStencil(void);
	virtual void Display(void);
	bool GetIsDirty(void);
};

#endif
