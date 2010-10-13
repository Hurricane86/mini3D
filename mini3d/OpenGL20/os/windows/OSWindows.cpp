#include "OSWindows.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OSWindows::OSWindows()
{
}

void mini3d::OSWindows::Init()
{
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetProgramiv = (PFNGLGETPROGRAMIVNVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

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

unsigned int mini3d::OSWindows::GetMonitorBitDepth() const
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

void mini3d::OSWindows::GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const
{
	// Get the size of the client area of the window 

	// TODO: Should be RECT somehow?
	RECT clientRectangle;
	GetClientRect(HWND(windowHandle), (LPRECT)&clientRectangle);

	// get the width and height (must be bigger than 0)
	width = clientRectangle.right - clientRectangle.left;
	height = clientRectangle.bottom - clientRectangle.top;

}

void mini3d::OSWindows::Log(wchar_t* message) const
{
	OutputDebugStringA((LPCSTR)message);
}
