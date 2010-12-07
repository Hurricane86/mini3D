
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#ifndef MINI3D_OSFUNCTIONLINUX_H
#define MINI3D_OSFUNCTIONLINUX_H

#define GLX_GLXEXT_PROTOTYPES
#include "../IOGLWrapper.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>


typedef char GLchar;

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
	virtual void PrepareWindow(const MINI3D_WINDOW window) const;
	virtual void SetRenderWindow(const MINI3D_WINDOW window) const;
	virtual void SetDefaultRenderWindow() const;
	virtual void SwapWindowBuffers(const MINI3D_WINDOW window) const;

private:

	// --------- Private helper functions ----------------------------------------

	Display* GetDisplayFromWindow(Window window) const;
	
	// Default window and render context
	Display* display; 
	XVisualInfo* vinfo; 
	XSetWindowAttributes swattr;
	Window window;
	GLXContext renderingContext;

};
}

#endif
#endif
