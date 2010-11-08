
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_ID3D9WINDOWRENDERTARGET_H
#define MINI3D_ID3D9WINDOWRENDERTARGET_H

#include "../IWindowRenderTarget.h"
#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"
#include <map>

namespace mini3d
{
class D3D9DepthStencil;
class D3D9WindowRenderTarget : public IWindowRenderTarget, public ID3D9RenderTarget, public ID3D9Resource
{


public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9WindowRenderTarget(D3D9GraphicsService* pGraphicsService, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~D3D9WindowRenderTarget(void);


	// ::::: IWindowRenderTarget ::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetWindowRenderTarget(const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);

	virtual unsigned int GetWidth() const { int w; w = screenState == SCREEN_STATE_WINDOWED ? width : fullscreenWidth; return w; };
	virtual unsigned int GetHeight() const { int h; h = screenState == SCREEN_STATE_WINDOWED ? height : fullscreenHeight; return h; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; }
	virtual int GetWindowHandle() const { return hWindow; };

	ScreenState GetScreenState() const { return screenState; }

	virtual void SetScreenStateWindowed();
	virtual void SetScreenStateFullscreen(const int& fullscreenWidth, const int& fullscreenHeight);

	virtual void Display();

	
	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }


	// ::::: ID3D9RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::::

	virtual bool GetFullscreenCompatible(void) const { return false; };
	virtual bool GetWindowedCompatible(void) const { return true; };
	
	IDirect3DSurface9*  GetRenderTargetBuffer(void) const { return pRenderTargetSurface; }
	virtual D3D9DepthStencil* GetDepthStencil(void) const { return pDepthStencil; }


public:

	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void SetScreenState(ScreenState value);
	virtual void SetFullscreenSize(const int& width, const int& height);


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void UpdateSize();
	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	D3D9GraphicsService* pGraphicsService;
	IDirect3DSwapChain9* pScreenRenderTarget;
	D3D9DepthStencil* pDepthStencil;

	IDirect3DSurface9* pRenderTargetSurface;

	int width;
	int height;

	unsigned int fullscreenWidth;
	unsigned int fullscreenHeight;

	int hWindow;
	bool depthTestEnabled;
	Quality quality;
	
	ScreenState screenState;
	
	WNDPROC pOrigProc;

	static std::map<int, D3D9WindowRenderTarget*> windowMap;

	int bufferWidth;
	int bufferHeight;
	int hBufferWindow;
	int bufferDepthTestEnabled;
	
	bool isDirty;

};
}

#endif
