
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifdef _WIN32

#include "OSFunctionsWindows.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OSFunctions::OSFunctions()
{
	CreateInternalWindow();
	CreateDevice();
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
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");
	
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetProgramiv = (PFNGLGETPROGRAMIVNVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");

	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");

	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");

	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");

	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");

	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");

	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
	glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
	glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");
}

void mini3d::OSFunctions::CreateDevice(void)
{
	// get the pixel format for the device context
	// query desktop video settings
	DEVMODE SDL_desktop_mode;
	EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, &SDL_desktop_mode);
	
	PIXELFORMATDESCRIPTOR pfd={0};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Make the pixel format compatible with opengl
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
    
	// Get the pixel format index for the new pixel format
	int iNewFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat( hDeviceContext, iNewFormat, &pfd);

	// Create a default reder context and set it as current so we can start loading graphics data to the graphics card
	hRenderContext = wglCreateContext(hDeviceContext);
	wglMakeCurrent(hDeviceContext, hRenderContext);

}

LRESULT CALLBACK OGL20WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void mini3d::OSFunctions::CreateInternalWindow(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = OGL20WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"OGL20InternalWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
		hWindow = 0;
		return;
    }

	hWindow = CreateWindowEx(WS_EX_CLIENTEDGE, L"OGL20InternalWindowClass", L"HiddenWindow", 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
	hDeviceContext = GetDC((HWND)hWindow);
}

void mini3d::OSFunctions::PrepareWindow(const int& hWindow) const
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	
	int colorBits = 24;
	int depthBits = 32;

	if (GetMonitorBitDepth() == 16)
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

	HDC hDC = GetDC((HWND)hWindow);
	SetPixelFormat(hDC, iFormat, &pfd);
}

void mini3d::OSFunctions::SetRenderWindow(const int& hWindow) const
{
	if (hWindow == 0)
	{
		wglMakeCurrent(hDeviceContext, hRenderContext);
	}
	else
	{
		HDC hDC = GetWindowDC((HWND)hWindow);
		ChangeDisplaySettings(NULL,0);	// Switch back to the desktop default resolution stored in registry
		ShowCursor(TRUE);	// Show mouse pointer
			
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		wglMakeCurrent(hDC, hRenderContext);
	}
}

void mini3d::OSFunctions::SetFullscreenRenderWindow(const int& hWindow, const unsigned int& width, const unsigned int& height) const
{
	// Set the video resolution to the fullscreen resolution

	DEVMODE dmScreenSettings = {0}; // Device Mode initialized to zero
	dmScreenSettings.dmSize=sizeof(dmScreenSettings); // Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth = width; // Selected Screen Width
	dmScreenSettings.dmPelsHeight = height;	// Selected Screen Height

	// if the size is zero, use the default desktop size
	if (dmScreenSettings.dmPelsWidth != 0 && dmScreenSettings.dmPelsWidth != 0)
	{
		dmScreenSettings.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;
	}

	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return;
	}

	HDC hDC = GetWindowDC((HWND)hWindow);
	GLBindFramebuffer(GL_FRAMEBUFFER, 0);
	wglMakeCurrent(hDC, hRenderContext);
}

void mini3d::OSFunctions::SetDefaultRenderWindow() const
{
	wglMakeCurrent(hDeviceContext, hRenderContext);
}

void mini3d::OSFunctions::SwapWindowBuffers(const int& hWindow) const
{
	HDC hDC = GetWindowDC((HWND)hWindow);
	SwapBuffers(hDC);
}

unsigned int mini3d::OSFunctions::GetMonitorBitDepth() const
{
	// read the monitor information from win32
	DEVMODE devMode;
	memset(&devMode, 0, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);

	// get monitor settings from os
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);

	switch(devMode.dmBitsPerPel)
	{
		case 16:
			return 16;
		case 32:
			return 32;
	}

	// Default case for when running 8 strange desktop modes. (I have no idea what mode that would be!)
	return 16;
}

void mini3d::OSFunctions::GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const
{
	// Get the size of the client area of the window 

	// TODO: Should be RECT somehow?
	RECT clientRectangle;
	GetClientRect(HWND(windowHandle), (LPRECT)&clientRectangle);

	// get the width and height (must be bigger than 0)
	width = clientRectangle.right - clientRectangle.left;
	height = clientRectangle.bottom - clientRectangle.top;

}

void mini3d::OSFunctions::Log(char* message) const
{
	OutputDebugStringA(message);
}

#endif