#include "DX9DepthStencil.h"
#include <d3d9.h>

DX9DepthStencil::DX9DepthStencil(DX9GraphicsService* pGraphicsService, unsigned int width, unsigned int height) : 
	pGraphicsService(pGraphicsService)
{
	SetDepthStencil(width, height);
	LoadResource();	
}

DX9DepthStencil::~DX9DepthStencil(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void DX9DepthStencil::SetDepthStencil(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->isDirty = true;
}
unsigned int DX9DepthStencil::GetWidth(void)
{
	return width;
}
unsigned int DX9DepthStencil::GetHeight(void)
{
	return height;
}
IDirect3DSurface9*  DX9DepthStencil::GetDepthStencilBuffer(void)
{
	return pDepthStencil;
}
void DX9DepthStencil::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pDepthStencil != 0 && (bufferWidth != width || bufferHeight != height)) // TODO: power of 2
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pDepthStencil == 0)
	{
		
		if( FAILED( pDevice->CreateDepthStencilSurface(width, height, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, true, &pDepthStencil, 0 ))) 
		{
			isDirty = true;
			return;
		}
	}

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void DX9DepthStencil::UnloadResource(void)
{
	if (pDepthStencil != 0)
	{
		pDepthStencil->Release();
		pDepthStencil = 0;
	}

	isDirty = true;
}

bool DX9DepthStencil::GetIsDirty(void)
{
	return isDirty;
}