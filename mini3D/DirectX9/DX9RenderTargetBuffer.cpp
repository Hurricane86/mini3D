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

#include "DX9RenderTargetBuffer.h"
#include <d3d9.h>

DX9RenderTargetBuffer::DX9RenderTargetBuffer(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow) : 
	pGraphicsService(pGraphicsService), pRenderTargetBuffer(0)
{
	SetRenderTargetBuffer(width, height, hWindow);
	LoadResource();	
}

DX9RenderTargetBuffer::~DX9RenderTargetBuffer(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void DX9RenderTargetBuffer::SetRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow)
{
	this->hWindow = hWindow;
	this->width = width;
	this->height = height;
	this->isDirty = true;
}
unsigned int DX9RenderTargetBuffer::GetWidth(void)
{
	return width;
}
unsigned int DX9RenderTargetBuffer::GetHeight(void)
{
	return height;
}
IDirect3DSurface9*  DX9RenderTargetBuffer::GetBackBuffer(void)
{
	IDirect3DSurface9* pRenderSurface;
	pRenderTargetBuffer->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);
	return pRenderSurface;
}
void DX9RenderTargetBuffer::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pRenderTargetBuffer != 0 && (bufferWidth != width || bufferHeight != height)) // TODO: power of 2
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pRenderTargetBuffer == 0)
	{
		D3DPRESENT_PARAMETERS pp = pGraphicsService->GetPresentationParameters();
		
		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = (HWND)hWindow;
				
		if( FAILED( pDevice->CreateAdditionalSwapChain(&pp, &pRenderTargetBuffer))) 
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

void DX9RenderTargetBuffer::UnloadResource(void)
{
	if (pRenderTargetBuffer != 0)
	{
		pRenderTargetBuffer->Release();
		pRenderTargetBuffer = 0;
	}

	isDirty = true;
}

bool DX9RenderTargetBuffer::GetIsDirty(void)
{
	return isDirty;
}