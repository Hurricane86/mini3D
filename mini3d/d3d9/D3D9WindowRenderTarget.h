
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

	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };

	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; }
	virtual int GetWindowHandle() const { return hWindow; };

	ScreenState GetScreenState() const { return screenState; }

	virtual void SetScreenStateWindowed();
	virtual void SetScreenStateFullscreen(const unsigned int& width, const unsigned int& height);

	virtual void Display();

	
	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }


	// ::::: ID3D9RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::::

	virtual bool GetFullscreenCompatible(void) const { return false; };
	virtual bool GetWindowedCompatible(void) const { return true; };
	
	IDirect3DSurface9*  GetRenderTargetBuffer(void) const { return pRenderTargetSurface; }
	virtual IDirect3DSurface9* GetDepthStencil(void) const { return pDepthStencil; }


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void UpdateSize();
	
	bool LoadRenderTarget(IDirect3DDevice9* pDevice);
	bool LoadDepthStencil(IDirect3DDevice9* pDevice);

	void UnloadRenderTarget();
	void UnloadDepthStencil();

	DEVMODE GetClosestCompatibleResolution(const unsigned int &width, const unsigned int &height);
	unsigned int ScoreDeviceModeMatch(const DEVMODE &dm1, const DEVMODE &dm2);

	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// Graphics service handle
	D3D9GraphicsService* pGraphicsService;
	
	// Hardware resources
	IDirect3DSwapChain9* pScreenRenderTarget;
	IDirect3DSurface9* pDepthStencil;
	IDirect3DSurface9* pRenderTargetSurface;

	// Size of the window
	int width;
	int height;

	// state parameters
	int hWindow;
	bool depthTestEnabled;
	Quality quality;
	
	// Screen state related parameter
	ScreenState screenState;
	
	// Stores the original window position and size before entering fullscreen
	RECT winRect;

	// Stores the windowstyle for the window before changing it when entering fullscreen
	long windowStyle;

	// The original windowproc of the OS window attached to this rendertarget
	WNDPROC pOrigProc;

	// A map matching window handles to the correct window render target object
	static std::map<int, D3D9WindowRenderTarget*> windowMap;

	// State of hardware resources
	bool isDirty;

};
}

#endif
