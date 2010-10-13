
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9PRESENTATIONPARAMETERS_H
#define MINI3D_DX9PRESENTATIONPARAMETERS_H

#include <d3d9.h>
#include "DX9GraphicsSettings.h"
#include "..\IWindowRenderTarget.h"
#include "..\error\Error.h"

namespace mini3d
{
class DX9PresentationParameters
{

private:
	D3DPRESENT_PARAMETERS presentationParameters;

public:
	DX9PresentationParameters()
	{
	}

	~DX9PresentationParameters()
	{
	}

	static D3DPRESENT_PARAMETERS GetPresentationParametersFromGraphicsSettings(IDirect3D9* pD3D, IWindowRenderTarget* pScreenRenderTarget)
	{
		// get the display mode
		D3DDISPLAYMODE d3ddm;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		// set the presentation parameters
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		// For storing the quality setting as defined in if statements below
		IWindowRenderTarget::Quality quality;

		// If pScreenRenderTarget != 0 then we have a screen render target that should be used as the default screen render target
		// This is used for fullscreen mode
		if (pScreenRenderTarget != 0)
		{
			quality = pScreenRenderTarget->GetQuality();
			// safe because only DX9FullscreenRenderTargets are assigned to pFullscreenRenderTarget
			d3dpp.BackBufferWidth = pScreenRenderTarget->GetWidth();
			d3dpp.BackBufferHeight = pScreenRenderTarget->GetHeight();
			d3dpp.Windowed = false;
			d3dpp.EnableAutoDepthStencil = pScreenRenderTarget->GetDepthTestEnabled();
		}
		// Create an "empty" default render target
		else
		{
			quality = IWindowRenderTarget::QUALITY_MINIMUM;
			d3dpp.BackBufferWidth = 1; // Default backbuffer should be 1x1 and is never used
			d3dpp.BackBufferHeight = 1; // Default backbuffer should be 1x1 and is never used
			d3dpp.Windowed = true;
			d3dpp.EnableAutoDepthStencil = false; // we manage swap chains manually
		}

		CheckMultisampleFormat(pD3D, quality, false);

		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferFormat = GetCorrectBackBufferFormat(pD3D); 
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.AutoDepthStencilFormat = GetCorrectDepthStencilFormat(pD3D); 
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.MultiSampleType = FromMultisampleFormat(quality);
		d3dpp.MultiSampleQuality = 0;

		return d3dpp;
	}


	static void CheckMultisampleFormat(IDirect3D9* pD3D, IWindowRenderTarget::Quality& quality, bool fullscreen)
	{

		D3DDISPLAYMODE displayMode;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	

		DWORD pQualityLevels;
		while (	quality > 0 && 
				FAILED(pD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
															D3DDEVTYPE_HAL, 
															displayMode.Format, 
															!fullscreen,
															FromMultisampleFormat(quality),
															&pQualityLevels)))
		{
			quality = (IWindowRenderTarget::Quality)((int)quality - 1);
		}
	}

	static D3DMULTISAMPLE_TYPE FromMultisampleFormat(IWindowRenderTarget::Quality quality)
	{
		switch(quality)
		{
		case IWindowRenderTarget::QUALITY_MINIMUM:
				return D3DMULTISAMPLE_NONE;
				break;
			case IWindowRenderTarget::QUALITY_LOW:
				return D3DMULTISAMPLE_2_SAMPLES;
				break;
			case IWindowRenderTarget::QUALITY_MEDIUM:
				return D3DMULTISAMPLE_4_SAMPLES;
				break;
			case IWindowRenderTarget::QUALITY_HIGH:
				return D3DMULTISAMPLE_8_SAMPLES;
				break;
			case IWindowRenderTarget::QUALITY_MAXIMUM:
				return D3DMULTISAMPLE_16_SAMPLES;
				break;
		}
		// default case, no multisample
		return D3DMULTISAMPLE_NONE;
	}


private: 

// Set up helper functions

	static D3DFORMAT GetCorrectBackBufferFormat(IDirect3D9* pD3D)
	{
		D3DDISPLAYMODE displayMode;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		// Get the color depth of the display
		if (displayMode.Format == D3DFMT_X8R8G8B8)
			return D3DFMT_X8R8G8B8;
		else if (displayMode.Format == D3DFMT_R5G6B5)
			return D3DFMT_R5G6B5;
		else 
			throw Error::MINI3D_ERROR_UNKNOWN_BACK_BUFFER_FORMAT;
	}


	static D3DFORMAT GetCorrectDepthStencilFormat(IDirect3D9* pD3D)
	{
		D3DDISPLAYMODE displayMode;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		// Get the color depth of the display
		D3DFORMAT displayFormat = GetCorrectBackBufferFormat(pD3D);

		// Check if we are running in 32 bit mode and 32 bit depth buffer is supported
		if(displayFormat == D3DFMT_X8R8G8B8)
		{
			if (SUCCEEDED(pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, displayMode.Format,  D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 )))
			{
				return D3DFMT_D32;
			}
			else if (SUCCEEDED(pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, displayMode.Format,  D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 )))
			{
				return D3DFMT_D24S8;
			}
		}
		// if we are only running 16 bit color format, then just run a 16 bit depth format...
		else if (displayFormat == D3DFMT_R5G6B5)
		{
			if (SUCCEEDED( pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, displayMode.Format,  D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
			{
				return D3DFMT_D16;
			}
		}

		// This is a "just in case" error. Alternative would be to set a 16bit detph stencil as the default case.
		// If this error ever shows up, it is time to work out a good default case.
		throw Error::MINI3D_ERROR_NO_SUPPORTED_DEPTH_STENCIL_FORMAT;
	}
};
}

#endif