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

mini3d::DX9ScreenRenderTarget::DX9ScreenRenderTarget(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality) : 
	pGraphicsService(pGraphicsService), pScreenRenderTarget(0), pDepthStencil(0), quality(quality)
{
	if (depthTestEnabled == true)
	{
		pDepthStencil = new DX9DepthStencil(pGraphicsService, width, height);
	}

	SetScreenRenderTarget(width, height, hWindow, depthTestEnabled, quality);
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

void mini3d::DX9ScreenRenderTarget::SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality)
{
	this->hWindow = hWindow;
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	this->isDirty = true;
}
unsigned int mini3d::DX9ScreenRenderTarget::GetWidth(void)
{
	return width;
}
unsigned int mini3d::DX9ScreenRenderTarget::GetHeight(void)
{
	return height;
}
bool mini3d::DX9ScreenRenderTarget::GetDepthTestEnabled(void)
{
	return depthTestEnabled;
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
IDirect3DSurface9*  mini3d::DX9ScreenRenderTarget::GetRenderTargetBuffer(void)
{
	IDirect3DSurface9* pRenderSurface;
	pScreenRenderTarget->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);
	return pRenderSurface;
}
mini3d::IDepthStencil*  mini3d::DX9ScreenRenderTarget::GetDepthStencil(void)
{
	return pDepthStencil;
}
void mini3d::DX9ScreenRenderTarget::LoadResource(void)
{
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
		
		pGraphicsService->CheckMultisampleFormat(quality, !pp.Windowed);

		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = (HWND)hWindow;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.MultiSampleQuality = pGraphicsService->FromMultisampleFormat(quality);
				
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

void mini3d::DX9ScreenRenderTarget::UnloadResource(void)
{
	if (pScreenRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		pScreenRenderTarget->Release();
		pScreenRenderTarget = 0;
	}

	isDirty = true;
}

bool mini3d::DX9ScreenRenderTarget::GetIsDirty(void)
{
	return isDirty;
}