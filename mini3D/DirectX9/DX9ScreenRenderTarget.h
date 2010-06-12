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
#include "internal/IDX9Resource.h"
#include <map>

namespace mini3d
{
class DX9DepthStencil;
class DX9ScreenRenderTarget : public IScreenRenderTarget, public IDX9RenderTarget, public IDX9Resource
{
// TODO: Public and private


public:
	DX9ScreenRenderTarget(DX9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~DX9ScreenRenderTarget(void);


	// ::::: IDepthStencil ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetScreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);

	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	virtual void SetSize(const int& width, const int& height);
	
	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; }
	virtual int GetWindowHandle() const { return hWindow; };

	virtual void Display();

	
	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }


	// ::::: IDX9RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::::

	virtual bool GetFullscreenCompatible(void) const { return false; };
	virtual bool GetWindowedCompatible(void) const { return true; };
	
	IDirect3DSurface9*  GetRenderTargetBuffer(void) const { return pRenderTargetSurface; }
	virtual IDepthStencil* GetDepthStencil(void) const { return pDepthStencil; }


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	DX9GraphicsService* pGraphicsService;
	IDirect3DSwapChain9* pScreenRenderTarget;
	IDepthStencil* pDepthStencil;

	IDirect3DSurface9* pRenderTargetSurface;

	int width;
	int height;
	int hWindow;
	bool depthTestEnabled;
	Quality quality;
	WNDPROC pOrigProc;

	static std::map<int, DX9ScreenRenderTarget*> windowMap;

	int bufferWidth;
	int bufferHeight;
	int hBufferWindow;
	int bufferDepthTestEnabled;
	
	bool isDirty;

};
}

#endif
