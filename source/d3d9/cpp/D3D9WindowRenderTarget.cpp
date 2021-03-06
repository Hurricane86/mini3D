
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "../D3D9WindowRenderTarget.h"
#include "../D3D9PresentationParameters.h"
#include "../../oswrapper/OSWrapper.h"
#include <d3d9.h>

std::map<HWND, mini3d::D3D9WindowRenderTarget*> mini3d::D3D9WindowRenderTarget::windowMap;

mini3d::D3D9WindowRenderTarget::D3D9WindowRenderTarget(D3D9GraphicsService* pGraphicsService, const HWND windowHandle, const bool& depthTestEnabled, const Quality& quality)
: 
pGraphicsService(pGraphicsService),
pScreenRenderTarget(0),
pDepthStencil(0),
quality(quality),
depthTestEnabled(depthTestEnabled),
pOrigProc(0),
screenState(SCREEN_STATE_WINDOWED),
oSWrapper(new OSWrapper)
{
	SetWindowRenderTarget(windowHandle, depthTestEnabled, quality);
	pGraphicsService->AddResource(this);
}

mini3d::D3D9WindowRenderTarget::~D3D9WindowRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
	
	if (pDepthStencil != 0)
		delete pDepthStencil;

	if (oSWrapper != 0)
		delete oSWrapper;

}

void mini3d::D3D9WindowRenderTarget::SetScreenStateWindowed()
{
	// Dont set the windowed screen state if it is already set
	if (screenState == SCREEN_STATE_WINDOWED)
		return;

	oSWrapper->RestoreFullscreenWindow(hWindow);

	// Keep track of our new state
	screenState = SCREEN_STATE_WINDOWED;

	if (oSWrapper != 0)
		delete oSWrapper;
}

void mini3d::D3D9WindowRenderTarget::SetScreenStateFullscreen(const unsigned int& width, const unsigned int& height)
{
	// Attempt to set the window to fullscreen mode
	oSWrapper->SetFullscreenWindow(hWindow, width, height);
	
	// Keep track of our new state
	screenState = SCREEN_STATE_FULLSCREEN;
}

void mini3d::D3D9WindowRenderTarget::SetWindowRenderTarget(const MINI3D_WINDOW windowHandle, const bool& depthTestEnabled, const Quality& quality)
{
	if (windowHandle != hWindow)
	{
		// if this is the first time we set this up, just replace the window process
		if (pOrigProc == 0)
		{
			// overwrite the window process for the window (our override window process will call the original window process saved in pOrigProc)
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			
			// make a map so the window process can find this class from the window handle
			windowMap.insert(std::pair<HWND, D3D9WindowRenderTarget*>(windowHandle, this));
		}
		// else we need to restore the old one first and then setup the new one
		else
		{
			// restore old
			(WNDPROC)SetWindowLongPtr((HWND)hWindow, GWL_WNDPROC, (LONG)&pOrigProc);
			windowMap.erase(hWindow);

			// set new
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			windowMap.insert(std::pair<HWND, D3D9WindowRenderTarget*>(windowHandle, this));
		}
	}

	// Get the size of the client area of the window 
	oSWrapper->GetWindowContentSize(windowHandle, width, height);

	// set the variables from the call
	this->hWindow = windowHandle;
	this->depthTestEnabled = depthTestEnabled;

	// load the buffer
	this->isDirty = true;
	LoadResource();
}

void mini3d::D3D9WindowRenderTarget::Display(void)
{
	if (pScreenRenderTarget == 0)
		return;

	/// Make sure we do an endScene before we present (DirectX9 specific).
	if (pGraphicsService->isDrawingScene == true)
		pGraphicsService->EndScene();

	pScreenRenderTarget->Present(0,0,0,0,0);
}

void mini3d::D3D9WindowRenderTarget::LoadResource(void)
{
	isDirty = true;

	// Get handle to device
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	
	// Check handle is valid
	if (pDevice == 0)
		return;

	// Check if this is the currently bound render target
	bool setRenderTargetToThis = (pGraphicsService->GetRenderTarget() == this);

	bool renderTargetIsDirty = !LoadRenderTarget(pDevice);
	bool depthStencilIsDirty = !LoadDepthStencil(pDevice);

	// restore rendertarget if neccessary
	if (setRenderTargetToThis == true)
		pGraphicsService->SetRenderTarget(this);
}

bool mini3d::D3D9WindowRenderTarget::LoadRenderTarget(IDirect3DDevice9* pDevice)
{
	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pScreenRenderTarget != 0)
	{
		D3DPRESENT_PARAMETERS pp;
		pScreenRenderTarget->GetPresentParameters(&pp);

		if (pp.BackBufferWidth != width || pp.BackBufferHeight != height)
		{
			UnloadRenderTarget();
		}
	}

	// If it does not exist, create a new one
	if (pScreenRenderTarget == 0)
	{
		D3DPRESENT_PARAMETERS pp;
		memcpy(&pp, &pGraphicsService->GetPresentationParameters(), sizeof(D3DPRESENT_PARAMETERS));

		D3D9PresentationParameters::CheckMultisampleFormat(pGraphicsService->GetDirect3D(), quality, pp.Windowed != 0);

		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = (HWND)hWindow;
		pp.MultiSampleType = D3D9PresentationParameters::FromMultisampleFormat(quality);
				
		if( FAILED( pDevice->CreateAdditionalSwapChain(&pp, &pScreenRenderTarget))) 
			return false;

		// store the rendertargetsurface to avoid reference counting
		pScreenRenderTarget->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderTargetSurface);
	}

	return true;
}

bool mini3d::D3D9WindowRenderTarget::LoadDepthStencil(IDirect3DDevice9* pDevice)
{
	// If depth test is disabled, unload the depth stencil and return ok!
	if (depthTestEnabled == false)
	{
		UnloadDepthStencil();
		return true;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pDepthStencil != 0)
	{
		D3DSURFACE_DESC desc;
		pDepthStencil->GetDesc(&desc);
		
		if (desc.Width != width || desc.Height != height)
		{
			UnloadDepthStencil();
		}
	}

	D3DPRESENT_PARAMETERS pp = pGraphicsService->GetPresentationParameters();

	// If it does not exist, create a new one
	if (pDepthStencil == 0)
	{
		if( FAILED( pDevice->CreateDepthStencilSurface(width, height, pp.AutoDepthStencilFormat, pp.MultiSampleType, pp.MultiSampleQuality, true, &pDepthStencil, 0 ))) 
		{
			return false;
		}
	}

	return true;
}


void mini3d::D3D9WindowRenderTarget::UnloadResource(void)
{
	UnloadRenderTarget();
	UnloadDepthStencil();

	isDirty = true;
}

void mini3d::D3D9WindowRenderTarget::UnloadRenderTarget(void)
{
	// if we are removing the current render target, restore the default render target first
	if (pGraphicsService->GetRenderTarget() == this)
		pGraphicsService->SetRenderTarget(0);

	if (pScreenRenderTarget != 0)
	{
		pRenderTargetSurface->Release();
		pRenderTargetSurface = 0;

		pScreenRenderTarget->Release();
		pScreenRenderTarget = 0;
	}
}

void mini3d::D3D9WindowRenderTarget::UnloadDepthStencil(void)
{
	// if we are removing the current render target, restore the default render target first
	if (pGraphicsService->GetRenderTarget() == this)
		pGraphicsService->SetRenderTarget(0);

	if (pDepthStencil != 0)
	{
		
		pDepthStencil->Release();
		pDepthStencil = 0;
	}
}

void mini3d::D3D9WindowRenderTarget::UpdateSize()
{
	SetWindowRenderTarget(hWindow, depthTestEnabled, quality);
}

LRESULT CALLBACK mini3d::D3D9WindowRenderTarget::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3D9WindowRenderTarget* screenRenderTarget = windowMap.find(hwnd)->second;

	switch(msg)
	{
	// Window has been resized
	case WM_SIZE:

			// Update the window size
			screenRenderTarget->width = LOWORD(lParam) | 1;
			screenRenderTarget->height = HIWORD(lParam) | 1;

			// update the render target size
			screenRenderTarget->LoadResource();
		break;
	}

	return CallWindowProc(screenRenderTarget->pOrigProc, hwnd, msg, wParam, lParam);
}

#endif
