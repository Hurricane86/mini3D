
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#include "OGLWrapperLinux.h"
#include <GL/glext.h>

mini3d::OGLWrapper::OGLWrapper()
{
	CreateDevice();
	CreateInternalWindow();
	Init();
}

mini3d::OGLWrapper::~OGLWrapper()
{
	
}


void mini3d::OGLWrapper::Init()
{
	glIsShader = (PFNGLISSHADERPROC)glXGetProcAddress((const GLubyte*)"glIsShader");
	
	glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const GLubyte*)"glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const GLubyte*)"glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const GLubyte*)"glCompileShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const GLubyte*)"glDeleteShader");

	glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const GLubyte*)"glGetShaderiv");
	glGetProgramiv = (PFNGLGETPROGRAMIVNVPROC)glXGetProcAddress((const GLubyte*)"glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetProgramInfoLog");

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glDeleteProgram");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glLinkProgram");
	
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glBindAttribLocation");

	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)glXGetProcAddress((const GLubyte*)"glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glGetAttribLocation");

	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glXGetProcAddress((const GLubyte*)"glGetActiveUniform");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
	
	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glValidateProgram");
	
	glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glUseProgram");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)glXGetProcAddress((const GLubyte*)"glActiveTexture");

	glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)glXGetProcAddress((const GLubyte*)"glGenRenderbuffersEXT");
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)glXGetProcAddress((const GLubyte*)"glBindRenderbufferEXT");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glXGetProcAddress((const GLubyte*)"glRenderbufferStorageEXT");

	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)glXGetProcAddress((const GLubyte*)"glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)glXGetProcAddress((const GLubyte*)"glBindFramebufferEXT");

	glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)glXGetProcAddress((const GLubyte*)"glDeleteRenderbuffersEXT");
	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)glXGetProcAddress((const GLubyte*)"glDeleteFramebuffersEXT");

	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)glXGetProcAddress((const GLubyte*)"glFramebufferTexture2DEXT");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glXGetProcAddress((const GLubyte*)"glFramebufferRenderbufferEXT");

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteBuffers");

	glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
	glMapBuffer = (PFNGLMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glUnmapBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
	glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");

	glUniform1f = (PFNGLUNIFORM1FPROC)glXGetProcAddress((const GLubyte*)"glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC)glXGetProcAddress((const GLubyte*)"glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC)glXGetProcAddress((const GLubyte*)"glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC)glXGetProcAddress((const GLubyte*)"glUniform4f");

	glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const GLubyte*)"glUniform1i");
	glUniform2i = (PFNGLUNIFORM2IPROC)glXGetProcAddress((const GLubyte*)"glUniform2i");
	glUniform3i = (PFNGLUNIFORM3IPROC)glXGetProcAddress((const GLubyte*)"glUniform3i");
	glUniform4i = (PFNGLUNIFORM4IPROC)glXGetProcAddress((const GLubyte*)"glUniform4i");

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const GLubyte*)"glUniformMatrix4fv");

}

void mini3d::OGLWrapper::CreateDevice(void)
{
	int attrList[20];
	int indx=0; 

	if (!(display = XOpenDisplay(0))) 
	{
		// todo: throw and error
		int i = 0;
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

	vinfo = glXChooseVisual(display, DefaultScreen(display), attrList);

	if (vinfo == NULL) 
	{ 
		// todo: throw error
	}

	swattr.colormap=XCreateColormap (display ,RootWindow (display,vinfo->screen), vinfo->visual, AllocNone);
	swattr.background_pixel = BlackPixel (display, vinfo->screen);
	swattr.border_pixel = BlackPixel (display, vinfo->screen);
}

void mini3d::OGLWrapper::CreateInternalWindow(void)
{
	    window = XCreateWindow(display, RootWindow(display, vinfo->screen), 30, 30, 64, 64, 0, vinfo->depth, CopyFromParent, vinfo->visual,CWBackPixel | CWBorderPixel | CWColormap, &swattr);
		renderingContext = glXCreateContext(display, vinfo, NULL, True);

		if (renderingContext == NULL)
		{ 
			// todo: throw error
		}

		if (!glXMakeCurrent(display, window, renderingContext))
		{ 
			// todo: throw error
		}
		
		int j = 0;
}

void mini3d::OGLWrapper::PrepareWindow(const MINI3D_WINDOW hWindow) const
{

}

void mini3d::OGLWrapper::SetRenderWindow(const MINI3D_WINDOW window) const
{
	if (window == 0)
	{
		if(!glXMakeCurrent(display, None, 0))
		{
			// todo: throw error
		}
	}
	else
	{
		//TODO: Switch to windowed mode
			
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (!glXMakeCurrent(GetDisplayFromWindow(window), window, renderingContext))
		{
			// todo: throw error
		}
	}
}

void mini3d::OGLWrapper::SetDefaultRenderWindow() const
{
	glXMakeCurrent(display, window, renderingContext);
}

void mini3d::OGLWrapper::SwapWindowBuffers(const MINI3D_WINDOW window) const
{
	glXSwapBuffers(GetDisplayFromWindow(window), window);
}

Display* mini3d::OGLWrapper::GetDisplayFromWindow(Window window) const
{
	XWindowAttributes attr;
	XGetWindowAttributes(display, window, &attr);
	
	return attr.screen->display;
}

#endif