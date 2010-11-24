
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifdef _WIN32

#include "OSFunctionsWindows.h"
#include <math.h>
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OSFunctions::OSFunctions() : fullscreenWindow(0)
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

	glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");

	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");

	glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");

	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");

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

void mini3d::OSFunctions::PrepareWindow(const MINI3D_WINDOW hWindow) const
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

void mini3d::OSFunctions::SetRenderWindow(const HWND hWindow) const
{
	if (hWindow == 0)
	{
		wglMakeCurrent(hDeviceContext, hRenderContext);
	}
	else
	{
		HDC hDC = GetWindowDC((HWND)hWindow);
		//ChangeDisplaySettings(NULL,0);	// Switch back to the desktop default resolution stored in registry
		//ShowCursor(TRUE);	// Show mouse pointer
			
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		wglMakeCurrent(hDC, hRenderContext);
	}
}

void mini3d::OSFunctions::SetFullscreenWindow(HWND hWindow, const unsigned int& width, const unsigned int& height)
{
	// Get the Device Mode that is closest to the requested resolution
	DEVMODE dm = GetClosestCompatibleResolution(width, height);

	// GetWindow client size
	RECT clientRect;
	GetClientRect((HWND)hWindow, &clientRect);

	int currentWidth = clientRect.right - clientRect.left;
	int currentHeight = clientRect.bottom - clientRect.top;

	// If we are already in fullscreen mode and the requested resolution is the same as the current one, dont set it again.
	if ((fullscreenWindow != 0) && (currentWidth == dm.dmPelsWidth) && (currentHeight == dm.dmPelsHeight))
		return;

	// if we are not already in fullscreen state, capture the original window settings before we change them
	if (fullscreenWindow == 0)
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
		// TODO: Handle Error?
		return;
	}

	// Keep track of new fullscreen window
	fullscreenWindow = hWindow;
}

void mini3d::OSFunctions::RestoreFullscreenWindow(HWND hWindow)
{
	// Restore window style
	SetWindowLongPtr((HWND)hWindow, GWL_STYLE, windowStyle);

	// Restore window size and position
	SetWindowPos((HWND)hWindow,
					HWND_TOP,
					winRect.left, 
					winRect.top, 
					winRect.right - winRect.left, 
					winRect.bottom - winRect.top, 
					SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Restore desktop resolution
	ChangeDisplaySettings(NULL, 0);

	// Set the fullscreenWindow to 0
	fullscreenWindow = 0;
}

DEVMODE mini3d::OSFunctions::GetClosestCompatibleResolution(const unsigned int& width, const unsigned int& height)
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

unsigned int mini3d::OSFunctions::ScoreDeviceModeMatch(const DEVMODE &dm1, const DEVMODE &dm2)
{
	// Score the similarity of the display modes by getting the difference between widths, heights and frequencies.
	// We get the total score when we add their absolute values together.
	unsigned int score = (unsigned int)(abs((double)(dm1.dmPelsWidth - dm2.dmPelsWidth)) + 
										abs((double)(dm1.dmPelsHeight - dm2.dmPelsHeight)) + 
										abs((double)(dm1.dmDisplayFrequency - dm2.dmDisplayFrequency)));

	return score;
}


void mini3d::OSFunctions::SetDefaultRenderWindow() const
{
	wglMakeCurrent(hDeviceContext, hRenderContext);
}

void mini3d::OSFunctions::SwapWindowBuffers(const HWND hWindow) const
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

void mini3d::OSFunctions::GetClientAreaSize(const HWND windowHandle, unsigned int &width, unsigned int &height) const
{
	// Get the size of the client area of the window 

	// TODO: Should be RECT somehow?
	RECT clientRectangle;
	GetClientRect(HWND(windowHandle), (LPRECT)&clientRectangle);

	// get the width and height (must be bigger than 0)
	width = clientRectangle.right - clientRectangle.left;
	height = clientRectangle.bottom - clientRectangle.top;

}

void mini3d::OSFunctions::Log(const char* message) const
{
	OutputDebugStringA(message);
}

#endif