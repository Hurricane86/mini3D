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

DX9ScreenRenderTarget::DX9ScreenRenderTarget(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0)
{
	if (depthTestEnabled == true)
	{
		pDepthStencil = new DX9DepthStencil(pGraphicsService, width, height);
	}

	SetScreenRenderTarget(width, height, hWindow, depthTestEnabled);
	LoadResource();	
}

DX9ScreenRenderTarget::~DX9ScreenRenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
	
	if (pDepthStencil != 0)
		delete pDepthStencil;
}

void DX9ScreenRenderTarget::SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled)
{
	this->hWindow = hWindow;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	this->isDirty = true;
}
unsigned int DX9ScreenRenderTarget::GetWidth(void)
{
	return width;
}
unsigned int DX9ScreenRenderTarget::GetHeight(void)
{
	return height;
}
bool DX9ScreenRenderTarget::GetDepthTestEnabled(void)
{
	return depthTestEnabled;
}
void DX9ScreenRenderTarget::Display(void)
{
	if (pScreenRenderTarget == 0)
		return;

	// TODO: Check so that graphicsSerivce is not in the middle of drawing

	pScreenRenderTarget->Present(0,0,0,0,0);
}
IDirect3DSurface9*  DX9ScreenRenderTarget::GetRenderTargetBuffer(void)
{
	IDirect3DSurface9* pRenderSurface;
	pScreenRenderTarget->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);
	return pRenderSurface;
}
IDepthStencil*  DX9ScreenRenderTarget::GetDepthStencil(void)
{
	return pDepthStencil;
}
void DX9ScreenRenderTarget::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pScreenRenderTarget != 0 && (bufferWidth != width || bufferHeight != height)) // TODO: power of 2
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pScreenRenderTarget == 0)
	{
		D3DPRESENT_PARAMETERS pp = pGraphicsService->GetPresentationParameters();
		
		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = (HWND)hWindow;
				
		if( FAILED( pDevice->CreateAdditionalSwapChain(&pp, &pScreenRenderTarget))) 
		{
			isDirty = true;
			return;
		}
	}

	bufferWidth = width;
	bufferHeight = height;
	hBufferWindow = hWindow;
	isDirty = false;
}

void DX9ScreenRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		pScreenRenderTarget->Release();
		pScreenRenderTarget = 0;
	}

	isDirty = true;
}

bool DX9ScreenRenderTarget::GetIsDirty(void)
{
	return isDirty;
}