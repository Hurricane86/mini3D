
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../D3D9RenderTargetTexture.h"
#include "../D3D9DepthStencil.h"
#include "../../error/error.h"
#include <d3d9.h>

mini3d::D3D9RenderTargetTexture::D3D9RenderTargetTexture(D3D9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) : 
	pGraphicsService(pGraphicsService), pDepthStencil(0), pRenderTarget(0)
{
	SetRenderTargetTexture(width, height, depthTestEnabled);
	pGraphicsService->AddResource(this);

}

mini3d::D3D9RenderTargetTexture::~D3D9RenderTargetTexture(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);

	if (pDepthStencil != 0)
		delete pDepthStencil;
}

void mini3d::D3D9RenderTargetTexture::SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{

	// If width or height is not a power of two
	if (((width & (width - 1)) != 0) || ((height & (height - 1)) != 0))
	{
		throw Error::MINI3D_ERROR_NON_POWER_OF_TWO;
	}

	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;

	this->isDirty = true;
	LoadResource();	

	// Update depth Stencil
	if (depthTestEnabled == true)
	{
		if (pDepthStencil == 0)
			pDepthStencil = new D3D9DepthStencil(pGraphicsService, width, height);
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
void mini3d::D3D9RenderTargetTexture::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pRenderTarget != 0 && (bufferWidth != width || bufferHeight != height))
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pRenderTarget == 0)
	{
		// X8R8G8B8
		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRenderTarget, 0 ) ) ) 
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

void mini3d::D3D9RenderTargetTexture::UnloadResource(void)
{
	if (pRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		// if we are removing one of the current textures, clear that texture slot first
		for(unsigned int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		pRenderTargetSurface->Release();
		pRenderTargetSurface = 0;

		pRenderTarget->Release();
		pRenderTarget = 0;
	}

	isDirty = true;
}
