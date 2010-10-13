
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../DX9DepthStencil.h"
#include <d3d9.h>

mini3d::DX9DepthStencil::DX9DepthStencil(DX9GraphicsService* pGraphicsService,  const unsigned int& width, const unsigned int& height) :
	pGraphicsService(pGraphicsService), pDepthStencil(0)
{
	SetDepthStencil(width, height);
	pGraphicsService->AddResource(this);
}

mini3d::DX9DepthStencil::~DX9DepthStencil(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::DX9DepthStencil::SetDepthStencil(const unsigned int& width, const unsigned int& height)
{
	this->width = width;
	this->height = height;
	
	this->isDirty = true;
	LoadResource();
}
void mini3d::DX9DepthStencil::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
	{
		isDirty = true;	
		return;
	}

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pDepthStencil != 0 && (bufferWidth != width || bufferHeight != height))
	{
		UnloadResource();
	}

	D3DPRESENT_PARAMETERS pp = pGraphicsService->GetPresentationParameters();

	// If it does not exist, create a new one
	if (pDepthStencil == 0)
	{
		
		if( FAILED( pDevice->CreateDepthStencilSurface(width, height, pp.AutoDepthStencilFormat, pp.MultiSampleType, pp.MultiSampleQuality, true, &pDepthStencil, 0 ))) 
		{
			isDirty = true;
			return;
		}
	}

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void mini3d::DX9DepthStencil::UnloadResource(void)
{
	if (pDepthStencil != 0)
	{
		// if this is the currently loaded index buffer, release it
		if (pGraphicsService->GetDepthStencil() == this)
			pGraphicsService->SetDepthStencil(0);

		pDepthStencil->Release();
		pDepthStencil = 0;
	}

	isDirty = true;
}
