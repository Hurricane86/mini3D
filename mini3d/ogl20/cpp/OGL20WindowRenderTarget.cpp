
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20WindowRenderTarget.h"
#include "../OGL20GraphicsService.h"
#include "../../error/Error.h"

std::map<MINI3D_WINDOW, mini3d::OGL20WindowRenderTarget*> mini3d::OGL20WindowRenderTarget::windowMap;

mini3d::OGL20WindowRenderTarget::OGL20WindowRenderTarget(OGL20GraphicsService* pGraphicsService, const MINI3D_WINDOW windowHandle, const bool& depthTestEnabled, const Quality& quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality), pOS(pGraphicsService->GetOS())
{
	screenState = SCREEN_STATE_WINDOWED;
	SetWindowRenderTarget(windowHandle, depthTestEnabled, quality);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20WindowRenderTarget::~OGL20WindowRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20WindowRenderTarget::SetScreenStateWindowed()
{
	// Dont set the windowed screen state if it is already set
	if (screenState == SCREEN_STATE_WINDOWED)
		return;

	// Set Windowed state and restore window
	pGraphicsService->GetOS()->RestoreFullscreenWindow(hWindow);

	// Keep track of our new state
	screenState = SCREEN_STATE_WINDOWED;

	// Set the viewport
	UpdateSize();

}

void mini3d::OGL20WindowRenderTarget::SetScreenStateFullscreen(const unsigned int& width, const unsigned int& height)
{
	// Set Windowed state and restore window
	pGraphicsService->GetOS()->SetFullscreenWindow(hWindow, width, height);

	// Keep track of our new state
	screenState = SCREEN_STATE_FULLSCREEN;

	// Set the viewport
	UpdateSize();
}

void mini3d::OGL20WindowRenderTarget::SetWindowRenderTarget(const MINI3D_WINDOW windowHandle, const bool& depthTestEnabled, const Quality& quality)
{

#ifdef _WIN32
	CaptureWindowProc(windowHandle);
#endif

	// Get the size of the client area of the window 
	unsigned int width, height;
	pGraphicsService->GetOS()->GetClientAreaSize(windowHandle, width, height);

	// set the variables from the call
	this->hWindow = windowHandle;
	
	// The width and height (must be bigger than 0)
	this->width = width | 1;
	this->height = height | 1;
	
	this->depthTestEnabled = depthTestEnabled;
	
	// load the buffer
	this->isDirty = true;
	LoadResource();
}

void mini3d::OGL20WindowRenderTarget::Display(void)
{
	pOS->SwapWindowBuffers(hWindow);
}

void mini3d::OGL20WindowRenderTarget::LoadResource(void)
{
	pOS->PrepareWindow(hWindow);

	hBufferWindow = hWindow;
	isDirty = false;
}

void mini3d::OGL20WindowRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);
	}

	isDirty = true;
}

void mini3d::OGL20WindowRenderTarget::UpdateSize()
{
	// if the rendertarget is currently set, update the viewport
	if (pGraphicsService->GetRenderTarget() == this)
	{
		pGraphicsService->GetOS()->GetClientAreaSize(hWindow, width, height);
		glViewport(0,0,width,height);
	}
}

#ifdef _WIN32

void mini3d::OGL20WindowRenderTarget::CaptureWindowProc(const MINI3D_WINDOW windowHandle)
{
	if (windowHandle != hWindow)
	{
		// if this is the first time we set this up, just replace the window process
		if (pOrigProc == 0)
		{
			// overwrite the window process for the window (our override window process will call the original window process saved in pOrigProc)
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			
			// make a map so the window process can find this class from the window handle
			windowMap.insert(std::pair<MINI3D_WINDOW, OGL20WindowRenderTarget*>(windowHandle, this));
		}
		// else we need to restore the old one first and then setup the new one
		else
		{
			// restore old
			(WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG)&pOrigProc);
			windowMap.erase(hWindow);

			// set new
			pOrigProc = (WNDPROC)SetWindowLongPtr(windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			windowMap.insert(std::pair<MINI3D_WINDOW,OGL20WindowRenderTarget*>(windowHandle, this));
		}
	}
}

LRESULT CALLBACK mini3d::OGL20WindowRenderTarget::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OGL20WindowRenderTarget* windowRenderTarget = windowMap.find(hwnd)->second;

	switch(msg)
	{
	// Window has been resized
	case WM_SIZE:
		
		// update the render target size
		windowRenderTarget->UpdateSize();
		
		break;
	}

	return CallWindowProc(windowRenderTarget->pOrigProc, hwnd, msg, wParam, lParam);
}

#endif
