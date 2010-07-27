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

#include "../OGL20WindowRenderTarget.h"
#include "../../error/Error.h"
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

std::map<int, mini3d::OGL20WindowRenderTarget*> mini3d::OGL20WindowRenderTarget::windowMap;

mini3d::OGL20WindowRenderTarget::OGL20WindowRenderTarget(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality)
{
	SetWindowRenderTarget(width, height, windowHandle, depthTestEnabled, quality);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20WindowRenderTarget::~OGL20WindowRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20WindowRenderTarget::SetWindowRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality)
{

	if (windowHandle != hWindow)
	{
		// if this is the first time we set this up, just replace the window process
		if (pOrigProc == 0)
		{
			// overwrite the window process for the window (our override window process will call the original window process saved in pOrigProc)
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			
			// make a map so the window process can find this class from the window handle
			windowMap.insert(std::pair<int, OGL20WindowRenderTarget*>(windowHandle, this));
		}
		// else we need to restore the old one first and then setup the new one
		else
		{
			// restore old
			(WNDPROC)SetWindowLongPtr((HWND)hWindow, GWL_WNDPROC, (LONG)&pOrigProc);
			windowMap.erase(hWindow);

			// set new
			pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)windowHandle, GWL_WNDPROC, (LONG)&HookWndProc);
			windowMap.insert(std::pair<int,OGL20WindowRenderTarget*>(windowHandle, this));
		}
	}

	// set the variables from the call
	this->hWindow = windowHandle;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	
	// load the buffer
	this->isDirty = true;
	LoadResource();
}

void mini3d::OGL20WindowRenderTarget::Display(void)
{
	if (hDeviceContext == 0)
		return;

	///// Make sure we do an endScene before we present (DirectX9 specific).
	//if (pGraphicsService->isDrawingScene == true)
	//	pGraphicsService->EndScene();

	SwapBuffers(hDeviceContext);
}

void mini3d::OGL20WindowRenderTarget::LoadResource(void)
{

	// get the device context (DC)
	hDeviceContext = GetDC((HWND)hWindow);

	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	int colorBits = 24;
	int depthBits = 32;

	if (pGraphicsService->GetOS()->GetMonitorBitDepth() == 16)
	{
		colorBits = depthBits = 16;
	}

    // set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = colorBits;
	pfd.cDepthBits = depthBits;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	if (!SetPixelFormat(hDeviceContext, iFormat, &pfd))
	{
		throw Error::MINI3D_ERROR_UNKNOWN;
	}

	bufferWidth = width;
	bufferHeight = height;
	hBufferWindow = hWindow;

	if (pGraphicsService->GetRenderContext() == 0)
	{
		pGraphicsService->SetRenderContext(wglCreateContext(hDeviceContext));
	}


	wglMakeCurrent(hDeviceContext, pGraphicsService->GetRenderContext());
	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	isDirty = false;
}

void mini3d::OGL20WindowRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		ReleaseDC( (HWND)hWindow, hDeviceContext );
		hDeviceContext = 0;
	}

	isDirty = true;
}

void mini3d::OGL20WindowRenderTarget::SetSize(const int& width, const int& height)
{
	SetWindowRenderTarget(width, height, hWindow, depthTestEnabled, quality);
}


LRESULT CALLBACK mini3d::OGL20WindowRenderTarget::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OGL20WindowRenderTarget* windowRenderTarget = windowMap.find((int)hwnd)->second;

	switch(msg)
	{
	// Window has been resized
	case WM_SIZE:
		
		// make sure we dont set the size to 0 (happens when we minimize the window)
		int width = LOWORD(lParam) | 1;
		int height = HIWORD(lParam) | 1;

		// update the render target size
		windowRenderTarget->SetSize(width, height);
		
		break;
	}

	return CallWindowProc(windowRenderTarget->pOrigProc, hwnd, msg, wParam, lParam);
}