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

#include "../DX9RenderTargetTexture.h"
#include "../DX9DepthStencil.h"
#include <d3d9.h>

mini3d::DX9RenderTargetTexture::DX9RenderTargetTexture(DX9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) : 
	pGraphicsService(pGraphicsService), pDepthStencil(0), pRenderTarget(0)
{
	SetRenderTarget(width, height, depthTestEnabled);
	pGraphicsService->AddResource(this);

}

mini3d::DX9RenderTargetTexture::~DX9RenderTargetTexture(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);

	if (pDepthStencil != 0)
		delete pDepthStencil;
}

void mini3d::DX9RenderTargetTexture::SetRenderTarget(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;

	this->isDirty = true;
	LoadResource();	

	// Update depth Stencil
	if (depthTestEnabled == true)
	{
		if (pDepthStencil == 0)
			pDepthStencil = new DX9DepthStencil(pGraphicsService, width, height);
		else
			pDepthStencil->SetDepthStencil(width, height);
	}
	else
	{
		if (pDepthStencil != 0)
		{
			delete pDepthStencil;
			pDepthStencil = 0;
		}
	}
}
void mini3d::DX9RenderTargetTexture::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pRenderTarget != 0 && (bufferWidth != width || bufferHeight != height)) // TODO: power of 2
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pRenderTarget == 0)
	{
		// X8R8G8B8
		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &pRenderTarget, 0 ) ) ) 
		{
			isDirty = true;
			return;
		}
		
		// Capture the render target surfrace to avoid reference counting in directx
		pRenderTarget->GetSurfaceLevel(0, &pRenderTargetSurface);
	}

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;

}

void mini3d::DX9RenderTargetTexture::UnloadResource(void)
{
	if (pRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		// if we are removing one of the current textures, clear that texture slot first
		for(int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		pRenderTargetSurface->Release();
		pRenderTargetSurface = 0;

		pRenderTarget->Release();
		pRenderTarget = 0;
	}

	isDirty = true;
}
