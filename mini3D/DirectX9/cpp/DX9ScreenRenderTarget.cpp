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

#include "../DX9ScreenRenderTarget.h"
#include "../DX9DepthStencil.h"
#include <d3d9.h>

std::map<int, mini3d::DX9ScreenRenderTarget*> mini3d::DX9ScreenRenderTarget::windowMap;
WNDPROC mini3d::DX9ScreenRenderTarget::pOrigProc;

mini3d::DX9ScreenRenderTarget::DX9ScreenRenderTarget(DX9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality), depthTestEnabled(depthTestEnabled)
{
	if (depthTestEnabled == true)
	{
		pDepthStencil = new DX9DepthStencil(pGraphicsService, width, height);
	}

	SetScreenRenderTarget(width, height, windowHandle, depthTestEnabled, quality);
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::DX9ScreenRenderTarget::~DX9ScreenRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
	
	if (pDepthStencil != 0)
		delete pDepthStencil;
}

void mini3d::DX9ScreenRenderTarget::SetScreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality)
{
	this->hWindow = windowHandle;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;

	windowMap.insert(std::pair<int, DX9ScreenRenderTarget*>(hWindow, this));

	pOrigProc = (WNDPROC)SetWindowLongPtr((HWND)hWindow, GWL_WNDPROC, (LONG)&HookWndProc);

	this->isDirty = true;
}
void mini3d::DX9ScreenRenderTarget::Display(void)
{
	if (pScreenRenderTarget == 0)
		return;

	/// Make sure we do an endScene before we present (DirectX9 specific).
	if (pGraphicsService->isDrawingScene == true)
		pGraphicsService->EndScene();

	pScreenRenderTarget->Present(0,0,0,0,0);
}
void mini3d::DX9ScreenRenderTarget::LoadResource(void)
{

	bool setRenderTargetToThis = false;

	if (pGraphicsService->GetRenderTarget() == this)
		setRenderTargetToThis = true;

	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pScreenRenderTarget != 0 && (bufferWidth != width || bufferHeight != height))
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pScreenRenderTarget == 0)
	{
		D3DPRESENT_PARAMETERS pp;
		memcpy(&pp, &pGraphicsService->GetPresentationParameters(), sizeof(D3DPRESENT_PARAMETERS));

		pGraphicsService->CheckMultisampleFormat(quality, pp.Windowed != 0);

		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = (HWND)hWindow;
		pp.MultiSampleType = pGraphicsService->FromMultisampleFormat(quality);
				
		if( FAILED( pDevice->CreateAdditionalSwapChain(&pp, &pScreenRenderTarget))) 
		{
			isDirty = true;
			return;
		}

		// store the rendertargetsurface to avoid reference counting
		pScreenRenderTarget->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderTargetSurface);
	}

	bufferWidth = width;
	bufferHeight = height;
	hBufferWindow = hWindow;
	isDirty = false;

	// restore rendertarget if neccessary
	if (setRenderTargetToThis == true && pGraphicsService->GetRenderTarget() != this)
		pGraphicsService->SetRenderTarget(this);

}

void mini3d::DX9ScreenRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		pRenderTargetSurface->Release();
		pRenderTargetSurface->Release();
		pRenderTargetSurface = 0;

		pScreenRenderTarget->Release();
		pScreenRenderTarget = 0;
	}

	isDirty = true;
}

void mini3d::DX9ScreenRenderTarget::SetSize(const int& width, const int& height)
{
	this->width = width;
	this->height = height;
	LoadResource();
	
	if (depthTestEnabled == true)
	{
		// TODO: this needs to be implemented in IDepthStencil!!
		// pDepthStencil->SetSize(width, height);
	}
}

LRESULT CALLBACK mini3d::DX9ScreenRenderTarget::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_SIZE:
		DX9ScreenRenderTarget* screenRenderTarget = windowMap.find((int)hwnd)->second;
		screenRenderTarget->SetSize(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	return CallWindowProc(pOrigProc, hwnd, msg, wParam, lParam);
}