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

#include "../OGL20FullscreenRenderTarget.h"
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20FullscreenRenderTarget::OGL20FullscreenRenderTarget(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality)
{
	SetFullscreenRenderTarget(width, height, windowHandle, depthTestEnabled, quality);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20FullscreenRenderTarget::~OGL20FullscreenRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20FullscreenRenderTarget::SetFullscreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality)
{
	// set the variables from the call
	this->hWindow = windowHandle;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	
	// load the buffer
	this->isDirty = true;
	LoadResource();
}

void mini3d::OGL20FullscreenRenderTarget::Display(void)
{
	if (hDeviceContext == 0)
		return;

	///// Make sure we do an endScene before we present (DirectX9 specific).
	//if (pGraphicsService->isDrawingScene == true)
	//	pGraphicsService->EndScene();

	SwapBuffers(hDeviceContext);
}

void mini3d::OGL20FullscreenRenderTarget::LoadResource(void)
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
		int i = 0;
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

void mini3d::OGL20FullscreenRenderTarget::UnloadResource(void)
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

void mini3d::OGL20FullscreenRenderTarget::SetSize(const int& width, const int& height)
{
	SetFullscreenRenderTarget(width, height, hWindow, depthTestEnabled, quality);
}

