
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_OSFUNCTIONWINDOWS_H
#define MINI3D_OSFUNCTIONWINDOWS_H

#include "../IOGLWrapper.h"
#include "../../../oswrapper/OSWrapper.h"

#include <windows.h>
#include <GL/wglext.h>


namespace mini3d
{
class OGLWrapper : public IOGLWrapper
{
public:
	
	OGLWrapper();

	virtual ~OGLWrapper();
	virtual void Init();

	// Device creation
	void CreateDevice();
	void CreateInternalWindow();


	// ---------- ABSTRACT OPENGL FUNCTIONS --------------------------------------
	
	// These functions preform opengl operations but they do not map 1-1 against opengl functions.
	// what they do is platform dependent
	virtual void PrepareWindow(const HWND hWindow) const;

	virtual void SetRenderWindow(const HWND hWindow) const;
	virtual void SetDefaultRenderWindow() const;

	virtual void SwapWindowBuffers(const HWND hWindow) const;

private:

	// --------- Private variables -----------------------------------------------

	// Default window and render context
	HWND hWindow;
	HDC hDeviceContext;
	HGLRC hRenderContext;

	// Current fullscreen window
	HWND fullscreenWindow;

	// Stores the original window position and size before entering fullscreen
	RECT winRect;

	// Stores the windowstyle for the window before changing it when entering fullscreen
	long windowStyle;

	// Exposes OS functionality
	OSWrapper* oSWrapper;
};
}

#endif
#endif