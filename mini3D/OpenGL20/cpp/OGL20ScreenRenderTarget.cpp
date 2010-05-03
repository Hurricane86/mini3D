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

#include "../OGL20ScreenRenderTarget.h"
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20ScreenRenderTarget::OGL20ScreenRenderTarget(OGL20GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality)
{
	SetScreenRenderTarget(width, height, hWindow, depthTestEnabled, quality);
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20ScreenRenderTarget::~OGL20ScreenRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20ScreenRenderTarget::SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality)
{
	this->hWindow = hWindow;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	this->isDirty = true;
}
unsigned int mini3d::OGL20ScreenRenderTarget::GetWidth(void)
{
	return width;
}
unsigned int mini3d::OGL20ScreenRenderTarget::GetHeight(void)
{
	return height;
}
bool mini3d::OGL20ScreenRenderTarget::GetDepthTestEnabled(void)
{
	return depthTestEnabled;
}
void mini3d::OGL20ScreenRenderTarget::Display(void)
{
	if (hDeviceContext == 0)
		return;

	/// Make sure we do an endScene before we present (DirectX9 specific).
	if (pGraphicsService->isDrawingScene == true)
		pGraphicsService->EndScene();

	SwapBuffers(hDeviceContext);
}
HDC  mini3d::OGL20ScreenRenderTarget::GetRenderTargetBuffer(void)
{
	return hDeviceContext;
}
void mini3d::OGL20ScreenRenderTarget::LoadResource(void)
{

	// get the device context (DC)
	hDeviceContext = GetDC((HWND)hWindow);

	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

    // set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16; // TODO: 16 vs 32 depth shizzle, get info from graphicsService
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	if (!SetPixelFormat(hDeviceContext, iFormat, &pfd))
	{
		int i = 0;
	}


	bufferWidth = width;
	bufferHeight = height;
	hBufferWindow = hWindow;
	isDirty = false;

	if (pGraphicsService->hRenderContext == 0)
	{
		pGraphicsService->hRenderContext=wglCreateContext(hDeviceContext);
	}


	if (!wglMakeCurrent(hDeviceContext, pGraphicsService->hRenderContext))
	{
		int i = 0;
	}

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix



	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();

	glTranslatef(-1.5f,0.0f,-6.0f);					// Move Left 1.5 Units And Into The Screen 6.0

	glBegin(GL_TRIANGLES);						// Drawing Using Triangles
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
		glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
		glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
	glEnd();

	SwapBuffers(hDeviceContext);

}

void mini3d::OGL20ScreenRenderTarget::UnloadResource(void)
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

bool mini3d::OGL20ScreenRenderTarget::GetIsDirty(void)
{
	return isDirty;
}