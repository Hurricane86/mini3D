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

namespace mini3d
{
class DX9FullscreenRenderTarget : public IFullscreenRenderTarget, public IDX9RenderTarget, public IDX9Resource
{

private:
	DX9GraphicsService* pGraphicsService;

	int width;
	int height;
	int hWindow;
	bool depthTestEnabled;
	Quality quality;

public:
	DX9FullscreenRenderTarget(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality) : 
	pGraphicsService(pGraphicsService), width(width), height(height), quality(quality), depthTestEnabled(depthTestEnabled), hWindow(hWindow)
	{
		
	}

	virtual ~DX9FullscreenRenderTarget(void){};

	virtual void SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality){};
	virtual unsigned int GetWidth(void) { return width; };
	virtual unsigned int GetHeight(void) { return height; };
	
	virtual void LoadResource(void) { return; };
	virtual void UnloadResource(void){ return; };
	virtual bool GetIsDirty(void) { return false; };

	virtual IDirect3DSurface9* GetRenderTargetBuffer(void) { return 0; };
	virtual IDepthStencil* GetDepthStencil(void) { return 0; };

	virtual bool GetDepthTestEnabled(void) { return depthTestEnabled; };
	virtual int GetWindow(void) { return hWindow; };
	virtual Quality GetQuality(void) { return quality; };
	virtual void Display(void);

	virtual bool GetFullscreenCompatible(void) { return true; };
	virtual bool GetWindowedCompatible(void) { return false; };
};
}
#endif