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

#ifndef AURORA_DX9ScreenRenderTarget_H
#define AURORA_DX9ScreenRenderTarget_H

#include "../IScreenRenderTarget.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

namespace mini3d
{
class DX9ScreenRenderTarget : public IScreenRenderTarget, public IDX9Resource
{
friend class DX9GraphicsService;

private:
	// Bitmap
	unsigned int width;
	unsigned int height;
	int hWindow;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	int hBufferWindow;
	IDirect3DSwapChain9* pScreenRenderTarget;
	
	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	virtual IDirect3DSurface9* GetBackBuffer(void);

public:
	DX9ScreenRenderTarget(DX9GraphicsService* graphicsService, unsigned int width, unsigned int height, int hWindow);
	~DX9ScreenRenderTarget(void);

	void SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow);
	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

};
}

#endif
