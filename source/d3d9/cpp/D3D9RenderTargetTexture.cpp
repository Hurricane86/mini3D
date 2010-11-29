
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "../D3D9RenderTargetTexture.h"
#include "../../mini3d/datatypes/error/error.h"
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
}

void mini3d::D3D9RenderTargetTexture::LoadResource(void)
{
	isDirty = true;	

	// Get handle to device
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	
	// Check handle is valid
	if (pDevice == 0)
		return;

	bool setRenderTargetToThis = (pGraphicsService->GetRenderTarget() == this);

	bool renderTargetIsDirty = !LoadRenderTarget(pDevice);
	bool depthStencilIsDirty = !LoadDepthStencil(pDevice);

	// restore rendertarget if neccessary
	if (setRenderTargetToThis == true && pGraphicsService->GetRenderTarget() != this)
		pGraphicsService->SetRenderTarget(this);

	isDirty = renderTargetIsDirty || depthStencilIsDirty;
}

bool mini3d::D3D9RenderTargetTexture::LoadRenderTarget(IDirect3DDevice9* pDevice)
{

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pRenderTarget != 0)
	{
		D3DSURFACE_DESC desc;
		pRenderTarget->GetLevelDesc(0, &desc);
		
		if (desc.Width != width || desc.Height != height)
		{
			UnloadRenderTarget();
		}
	}

	// If it does not exist, create a new one
	if (pRenderTarget == 0)
	{
		// X8R8G8B8
		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRenderTarget, 0 ) ) ) 
		{
			return false;
		}
		
		// Capture the render target surfrace to avoid reference counting in directx
		pRenderTarget->GetSurfaceLevel(0, &pRenderTargetSurface);
	}

	return true;
}

bool mini3d::D3D9RenderTargetTexture::LoadDepthStencil(IDirect3DDevice9* pDevice)
{
	// If depth test is disabled, unload the depth stencil and return ok!
	if (depthTestEnabled == false)
	{
		UnloadDepthStencil();
		return true;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pDepthStencil != 0)
	{
		D3DSURFACE_DESC desc;
		pDepthStencil->GetDesc(&desc);
		
		if (desc.Width != width || desc.Height != height)
		{
			UnloadDepthStencil();
		}
	}

	D3DPRESENT_PARAMETERS pp = pGraphicsService->GetPresentationParameters();

	// If it does not exist, create a new one
	if (pDepthStencil == 0)
	{
		if( FAILED( pDevice->CreateDepthStencilSurface(width, height, pp.AutoDepthStencilFormat, pp.MultiSampleType, pp.MultiSampleQuality, true, &pDepthStencil, 0 ))) 
		{
			return false;
		}
	}

	return true;
}

void mini3d::D3D9RenderTargetTexture::UnloadResource()
{
	UnloadRenderTarget();
	UnloadDepthStencil();

	isDirty = true;
}


void mini3d::D3D9RenderTargetTexture::UnloadRenderTarget()
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
}

void mini3d::D3D9RenderTargetTexture::UnloadDepthStencil()
{
	if (pDepthStencil != 0)
	{
		pDepthStencil->Release();
		pDepthStencil = 0;
	}
}

#endif