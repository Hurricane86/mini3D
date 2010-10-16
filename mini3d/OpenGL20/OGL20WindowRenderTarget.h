
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IOGL20WINDOWRENDERTARGET_H
#define MINI3D_IOGL20WINDOWRENDERTARGET_H

#include "../IWindowRenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"
#include "os/OSFunctionSelector.h"
#include <map>


namespace mini3d
{
class OGL20DepthStencil;
class OGL20WindowRenderTarget : public IWindowRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20WindowRenderTarget(OGL20GraphicsService* pGraphicsService, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	virtual ~OGL20WindowRenderTarget(void);


	// ::::: IWindowRenderTarget ::::::::::::::::::::::::::::::::::::::::::::::

	virtual void SetWindowRenderTarget(const int& windowHandle, const bool& depthTestEnabled, const Quality& quality);
	
	virtual unsigned int GetWidth() const { return width; };
	virtual unsigned int GetHeight() const { return height; };
	
	virtual bool GetDepthTestEnabled() const { return depthTestEnabled; };
	
	virtual Quality GetQuality() const { return quality; };
	
	virtual int GetWindowHandle() const { return hWindow; };

	ScreenState GetScreenState() const { return screenState; }

	virtual void SetScreenStateWindowed();
	virtual void SetScreenStateFullscreen(const int& fullscreenWidth, const int& fullscreenHeight);

	virtual void Display();


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; }

	
	// ::::: IOGL20RenderTarget :::::::::::::::::::::::::::::::::::::::::::::::

	HDC GetDeviceContext(void) const { return GetDC((HWND)hWindow); }
//	HGLRC GetRenderContext(void) const { return hRenderContext; }
	GLuint GetDepthStencil(void) const { return pDepthStencil; }

public:

	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void SetScreenState(ScreenState value);

	virtual unsigned int GetFullscreenWidth() const { return fullscreenWidth; };
	virtual unsigned int GetFullscreenHeight() const { return fullscreenHeight; };
	virtual void SetFullscreenSize(const int& width, const int& height);


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void UpdateSize();

#ifdef _WIN32
	void CaptureWindowProc(const int& windowHandle);
	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	OGL20GraphicsService* pGraphicsService;
	OSFunctions* pOS;

	// This is a link to a windows device context
	//HGLRC hRenderContext;

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
	
	int hBufferWindow;
	int bufferDepthTestEnabled;

	ScreenState screenState;

	GLuint pScreenRenderTarget;

	bool isDirty;

};
}

#endif
