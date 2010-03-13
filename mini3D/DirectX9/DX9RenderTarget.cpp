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