
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

	// Restore window style
	SetWindowLongPtr((HWND)hWindow, GWL_STYLE, windowStyle);

	// Restore window size and position
	SetWindowPos((HWND)hWindow,
				 HWND_TOPMOST,
				 winRect.left, 
				 winRect.top, 
				 winRect.right - winRect.left, 
				 winRect.bottom - winRect.top, 
				 SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Restore desktop resolution
	ChangeDisplaySettings(NULL, 0);

	// Keep track of our new state
	screenState = SCREEN_STATE_WINDOWED;

	// Set the viewport
	UpdateSize();

}

void mini3d::OGL20WindowRenderTarget::SetScreenStateFullscreen(const unsigned int& width, const unsigned int& height)
{
	// Get the Device Mode that is closest to the requested resolution
	DEVMODE dm = GetClosestCompatibleResolution(width, height);

	// If we are already in fullscreen mode and the requested resolution is the same as the current one, dont set it again.
	if ((screenState == SCREEN_STATE_FULLSCREEN) && (this->width = dm.dmPelsWidth) && (this->height = dm.dmPelsHeight))
		return;

	// if we are not already in fullscreen state, capture the original window settings before we change them
	if (screenState == SCREEN_STATE_WINDOWED)
	{
		// Capture window style
		windowStyle = GetWindowLongPtr((HWND)hWindow, GWL_STYLE);
			
		// Capture window position
		GetWindowRect((HWND)hWindow, &winRect);
	}

	// Make the window fullscreen and the same size as the fullscreen desktop
	SetWindowLongPtr((HWND)hWindow, GWL_STYLE, WS_POPUP);
	SetWindowPos((HWND)hWindow, HWND_TOPMOST, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Change Screen Resolution
	if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{      
		MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
		return;
	}

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

DEVMODE mini3d::OGL20WindowRenderTarget::GetClosestCompatibleResolution(const unsigned int &width, const unsigned int &height)
{

	// Get the current device mode
	DEVMODE currentDM = {0};
	currentDM.dmSize = sizeof(currentDM);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currentDM);

	// If width or height = 0 set them to the current desktop resolution
	if (width == 0 || height == 0)
		return currentDM;

	// initialize the device mode structure for requested device mode
	DEVMODE requestedDM = currentDM;
	requestedDM.dmPelsWidth = width;
	requestedDM.dmPelsHeight = height;

	// initialize the device mode structure for best match
	DEVMODE bestDM = currentDM;

	// Difference in area between best match and 
	unsigned int bestMatchAreaDifference = ScoreDeviceModeMatch(requestedDM, currentDM); 

	// initialize the device mode structure for looping over all device modes
	DEVMODE dm = {0};
	dm.dmSize = sizeof(dm);

	// loop variable
	unsigned int i = 0;

	// Loop over all display settings and find the best match
	// EnumDisplaySettings returns 0 when we request a displaymode id that is out of range
	while (EnumDisplaySettings(NULL, i++, &dm) != 0)
	{
		// skip modes with wrong color bit depth
		if (dm.dmBitsPerPel != currentDM.dmBitsPerPel)
			continue;

		// skip modes with wrong display orientation
		if (dm.dmOrientation != currentDM.dmOrientation)
			continue;

		unsigned int diff = ScoreDeviceModeMatch(requestedDM, dm);

		if (diff < bestMatchAreaDifference)
		{
			bestDM = dm;
			bestMatchAreaDifference = diff;
		}
	}
	
	// Return the best match found
	return bestDM;
}

unsigned int mini3d::OGL20WindowRenderTarget::ScoreDeviceModeMatch(const DEVMODE &dm1, const DEVMODE &dm2)
{
	// Score the similarity of the display modes by getting the difference between widths, heights and frequencies.
	// We get the total score when we add their absolute values together.
	unsigned int score = (unsigned int)(abs((double)(dm1.dmPelsWidth - dm2.dmPelsWidth)) + 
										abs((double)(dm1.dmPelsHeight - dm2.dmPelsHeight)) + 
										abs((double)(dm1.dmDisplayFrequency - dm2.dmDisplayFrequency)));

	return score;
}

#endif
