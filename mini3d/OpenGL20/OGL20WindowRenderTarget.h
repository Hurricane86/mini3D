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

#ifndef AURORA_IOGL20WINDOWRENDERTARGET_H
#define AURORA_IOGL20WINDOWRENDERTARGET_H

#include "../IWindowRenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"
#include <Windows.h>
#include <GL/gl.h>
#include <map>


namespace mini3d
{
class OGL20DepthStencil;
class OGL20WindowRenderTarget : public IWindowRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20WindowRenderTarget(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~OGL20WindowRenderTarget(void);


	// ::::: IWindowRenderTarget ::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetWindowRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	virtual void SetSize(const int& width, const int& height);

	virtual unsigned int GetFullscreenWidth() const { return fullscreenWidth; };
	virtual unsigned int GetFullscreenHeight() const { return fullscreenHeight; };
	virtual void SetFullscreenSize(const int& width, const int& height);
	
	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	virtual Quality GetQuality() const { return quality; };
	virtual int GetWindowHandle() const { return hWindow; };

	ScreenState GetScreenState() const { return screenState; }
	void SetScreenState(ScreenState value);


	virtual void Display();


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }

	
	// ::::: IOGL20RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::

	HDC GetDeviceContext(void) const { return GetDC((HWND)hWindow); }
	HGLRC GetRenderContext(void) const { return hRenderContext; }
	GLuint GetDepthStencil(void) const { return pDepthStencil; }


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	OGL20GraphicsService* pGraphicsService;

	// The device context for the window
	HDC hDeviceContext;

	// This is a link to a windows device context
	HGLRC hRenderContext;

	GLuint pDepthStencil;

	unsigned int width;
	unsigned int height;

	unsigned int fullscreenWidth;
	unsigned int fullscreenHeight;

	int hWindow;

	bool depthTestEnabled;
	Quality quality;

	WNDPROC pOrigProc;
	static std::map<int, OGL20WindowRenderTarget*> windowMap;

	unsigned int bufferWidth;
	unsigned int bufferHeight;
	
	int hBufferWindow;
	int bufferDepthTestEnabled;

	ScreenState screenState;

	GLuint pScreenRenderTarget;

	bool isDirty;

};
}

#endif
