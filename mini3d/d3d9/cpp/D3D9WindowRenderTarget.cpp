
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>



#include "../D3D9WindowRenderTarget.h"
#include "../D3D9DepthStencil.h"
#include "../D3D9PresentationParameters.h"
#include <d3d9.h>

std::map<int, mini3d::D3D9WindowRenderTarget*> mini3d::D3D9WindowRenderTarget::windowMap;

mini3d::D3D9WindowRenderTarget::D3D9WindowRenderTarget(D3D9GraphicsService* pGraphicsService, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) : 
pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality), depthTestEnabled(depthTestEnabled), pOrigProc(0), fullscreenWidth(1680), fullscreenHeight(1050)
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
}

void mini3d::D3D9WindowRenderTarget::SetFullscreenSize(const int& width, const int& height)
{ 

	if (fullscreenWidth == width && fullscreenHeight == height)
		return;
	
	fullscreenWidth = width; 
	fullscreenHeight = height;

	if (pGraphicsService->GetRenderTarget() == this) 
		pGraphicsService->SetRenderTarget(this); 
}

void mini3d::D3D9WindowRenderTarget::SetScreenStateWindowed()
{
	SetScreenState(SCREEN_STATE_WINDOWED);
}

void mini3d::D3D9WindowRenderTarget::SetScreenStateFullscreen(const int& fullscreenWidth, const int& fullscreenHeight)
{
	this->fullscreenWidth = fullscreenWidth;
	this->fullscreenHeight = fullscreenHeight;

	SetScreenState(SCREEN_STATE_FULLSCREEN);
}


void mini3d::D3D9WindowRenderTarget::SetScreenState(ScreenState value)
{ 
	if (screenState == value)
		return;

	screenState = value;

	if (pGraphicsService->GetRenderTarget() == this) 
		pGraphicsService->SetRenderTarget(this);
}

void mini3d::D3D9WindowRenderTarget::SetWindowRenderTarget(const int& windowHandle, const bool& depthTestEnabled, const Quality& quality)
{

	if (windowHandle != hWindow)
	{
		// if this is the first time we set this up, just replace the window process
		if (pOrigProc == 0)
		{
			// overwrite the window process for the window (our override window process will call the original window process saved in pOrigProc)
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			
			// make a map so the window process can find this class from the window handle
			windowMap.insert(std::pair<int, D3D9WindowRenderTarget*>(windowHandle, this));
		}
		// else we need to restore the old one first and then setup the new one
		else
		{
			// restore old
			(WNDPROC)SetWindowLongPtr((HWND)hWindow, GWL_WNDPROC, (LONG)&pOrigProc);
			windowMap.erase(hWindow);

			// set new
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			windowMap.insert(std::pair<int, D3D9WindowRenderTarget*>(windowHandle, this));
		}
	}

	// Get the size of the client area of the window 
	LPRECT clientRectangle = new tagRECT();
	GetClientRect(HWND(windowHandle), clientRectangle);

	// get the width and height (must be bigger than 0)
	int width = (clientRectangle->right - clientRectangle->left) | 1;
	int height = (clientRectangle->bottom - clientRectangle->top) | 1;

	delete clientRectangle;

	// set the variables from the call
	this->hWindow = windowHandle;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	
	// load the buffer
	this->isDirty = true;
	LoadResource();

	// Change the settings if we are in windowed mode
	if (pGraphicsService->GetIsFullScreen() == false)
	{
		// Create/Update depth Stencil as needed
		if (depthTestEnabled == true)
		{
			if (pDepthStencil == 0)
				pDepthStencil = new D3D9DepthStencil(pGraphicsService, width, height);
			else
				pDepthStencil->SetDepthStencil(width, height);
		}
		else
		{
			if (pDepthStencil != 0)
			{
				delete pDepthStencil;
				pDepthStencil = 0;
			}
		}
	}

}
void mini3d::D3D9WindowRenderTarget::Display(void)
{
	if (pScreenRenderTarget == 0)
		return;

	/// Make sure we do an endScene before we present (DirectX9 specific).
	if (pGraphicsService->isDrawingScene == true)
		pGraphicsService->EndScene();

	if (screenState == SCREEN_STATE_FULLSCREEN)
		pGraphicsService->GetDevice()->Present(0,0,0,0);
	else
		pScreenRenderTarget->Present(0,0,0,0,0);
}
void mini3d::D3D9WindowRenderTarget::LoadResource(void)
{

	if (pGraphicsService->GetIsFullScreen() == true)
	{
		return;
	}

	bool setRenderTargetToThis = false;

	if (pGraphicsService->GetRenderTarget() == this)
		setRenderTargetToThis = true;

	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pScreenRenderTarget != 0 && (bufferWidth != width || bufferHeight != height))
	{
		UnloadResource();
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
		{
			isDirty = true;
			return;
		}

		// store the rendertargetsurface to avoid reference counting
		pScreenRenderTarget->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderTargetSurface);
	}

	bufferWidth = width;
	bufferHeight = height;
	hBufferWindow = hWindow;
	isDirty = false;

	// restore rendertarget if neccessary
	if (setRenderTargetToThis == true && pGraphicsService->GetRenderTarget() != this)
		pGraphicsService->SetRenderTarget(this);

}

void mini3d::D3D9WindowRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		pRenderTargetSurface->Release();
		pRenderTargetSurface = 0;

		pScreenRenderTarget->Release();
		pScreenRenderTarget = 0;
	}

	isDirty = true;
}

void mini3d::D3D9WindowRenderTarget::UpdateSize()
{
	SetWindowRenderTarget(hWindow, depthTestEnabled, quality);
}

LRESULT CALLBACK mini3d::D3D9WindowRenderTarget::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3D9WindowRenderTarget* screenRenderTarget = windowMap.find((int)hwnd)->second;

	switch(msg)
	{
	// Window has been resized
	case WM_SIZE:

		// update the render target size
		screenRenderTarget->UpdateSize();
		break;
	}

	return CallWindowProc(screenRenderTarget->pOrigProc, hwnd, msg, wParam, lParam);
}