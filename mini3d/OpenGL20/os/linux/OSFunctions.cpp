
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#include "OSFunctions.h"
#include <GL/glext.h>

mini3d::OSFunctions::OSFunctions()
{
	CreateDevice();
	CreateInternalWindow();
	Init();
}

mini3d::OSFunctions::~OSFunctions()
{
	// Remove the default device context
	DeleteDC(hDeviceContext);

	// Clear the device/render context setting
    wglMakeCurrent(0, 0);

	// delete the default render context
	wglDeleteContext(hRenderContext);
}


void mini3d::OSFunctions::Init()
{
	// Does nothing
}

void mini3d::OSFunctions::CreateDevice(void)
{
	int attrList[20];
	int indx=0; 

	if (!(display = XOpenDisplay (""))) 
	{
		// todo: throw and error
	}

	attrList[indx] = GLX_USE_GL; indx++;
	attrList[indx] = GLX_DEPTH_SIZE; indx++;
	attrList[indx] = 1; indx++;
	attrList[indx] = GLX_RGBA; indx++;
	attrList[indx] = GLX_RED_SIZE; indx++;
	attrList[indx] = 1; indx++;
	attrList[indx] = GLX_GREEN_SIZE; indx++;
	attrList[indx] = 1; indx++;
	attrList[indx] = GLX_BLUE_SIZE; indx++;
	attrList[indx] = 1; indx++;
	attrList[indx] = None;

	vinfo = glXChooseVisual(display, DefaultScreen(dpy), attrList);

	if (vinfo == NULL) 
	{ 
		// todo: throw error
	}

	swattr.colormap=XCreateColormap (display ,RootWindow (display,vinfo->screen), vinfo->visual, AllocNone);
	swattr.background_pixel = BlackPixel (display, vinfo->screen);
	swattr.border_pixel = BlackPixel (display, vinfo->screen);
}

void mini3d::OSFunctions::CreateInternalWindow(void)
{
	    wid = XCreateWindow(display, RootWindow(display, vinfo->screen), 30, 30, width, height, 0, vinfo->depth, CopyFromParent, vinfo->visual,CWBackPixel | CWBorderPixel | CWColormap, &swattr);
		renderingContext = glXCreateContext(display, vinfo, NULL, True);

		if (renderingContext == NULL)
		{ 
			// todo: throw error
		}

		if (!glXMakeCurrent(display, wid, renderingContext))
		{ 
			// todo: throw error
		} 
}

void mini3d::OSFunctions::PrepareWindow(const int& hWindow) const
{

}

void mini3d::OSFunctions::SetRenderWindow(const int& hWindow) const
{
	if (hWindow == 0)
	{
		glXMakeCurrent(display, None, 0);
	}
	else
	{
		HDC hDC = GetWindowDC((HWND)hWindow);
		ChangeDisplaySettings(NULL,0);	// Switch back to the desktop default resolution stored in registry
		ShowCursor(TRUE);	// Show mouse pointer
			
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		glXMakeCurrent(display, hWindow, renderingContext);
	}
}

void mini3d::OSFunctions::SetFullscreenRenderWindow(const int& hWindow, const unsigned int& width, const unsigned int& height) const
{
	// Set the video resolution to the fullscreen resolution
	
	// TODO: Set Fullscreen mode
	GLBindFramebuffer(GL_FRAMEBUFFER, 0);
	glXMakeCurrent(display, hWindow, renderingContext);
}

void mini3d::OSFunctions::SetDefaultRenderWindow() const
{
	glXMakeCurrent(display, hWindow, renderingContext);
}

void mini3d::OSFunctions::SwapWindowBuffers(const int& hWindow) const
{
	glXSwapBuffers(display, hWindow)
}

unsigned int mini3d::OSFunctions::GetMonitorBitDepth() const
{
	return 32;
}

void mini3d::OSFunctions::GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const
{
	XWindowAttributes attr;
	XGetWindowAttributes( display, windowHandle, &attr);
	
	width = attr.width;
	height = attr.height;
}

void mini3d::OSFunctions::Log(char* message) const
{
	printf(message);
}

#endif