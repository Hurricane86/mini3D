#include "DX9RenderTargetBuffer.h"
#include <d3d9.h>

DX9RenderTargetBuffer::DX9RenderTargetBuffer(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height, int hWindow) : 
	pGraphicsService(pGraphicsService)
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
				
		if( FAILED( pDevice->CreateAdditionalSwapChain(&pp, &pRenderTargetBuffer))) 
		{
			isDirty = true;
			return;
		}
	}

	bufferWidth = width;
	bufferHeight = height;
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