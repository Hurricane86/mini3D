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

#include "DX9RenderTarget.h"
#include <d3d9.h>

DX9RenderTarget::DX9RenderTarget(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height) : 
	pGraphicsService(pGraphicsService)
{
	SetRenderTarget(width, height);
	LoadResource();	
}

DX9RenderTarget::~DX9RenderTarget(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void DX9RenderTarget::SetRenderTarget(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->isDirty = true;
}
unsigned int DX9RenderTarget::GetWidth(void)
{
	return width;
}
unsigned int DX9RenderTarget::GetHeight(void)
{
	return height;
}
IDirect3DSurface9*  DX9RenderTarget::GetRenderTargetBuffer(void)
{
	IDirect3DSurface9* pSurface;
	pRenderTarget->GetSurfaceLevel(0, &pSurface);

	return pSurface;
}
IDirect3DTexture9*  DX9RenderTarget::GetTextureBuffer(void)
{
	return pRenderTarget;
}
void DX9RenderTarget::LoadResource(void)
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
		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRenderTarget, 0 ) ) ) 
		{
			isDirty = true;
			return;
		}
	}

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void DX9RenderTarget::UnloadResource(void)
{
	if (pRenderTarget != 0)
	{
		pRenderTarget->Release();
		pRenderTarget = 0;
	}

	isDirty = true;
}

bool DX9RenderTarget::GetIsDirty(void)
{
	return isDirty;
}