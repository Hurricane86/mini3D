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

#ifndef AURORA_DX9PRESENTATIONPARAMETERS_H
#define AURORA_DX9PRESENTATIONPARAMETERS_H

#include <d3d9.h>
#include "DX9GraphicsSettings.h"
#include "..\internal\IScreenRenderTarget.h"

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

	static D3DPRESENT_PARAMETERS GetPresentationParametersFromGraphicsSettings(IDirect3D9* pD3D, IScreenRenderTarget* pScreenRenderTarget)
	{
		// get the display mode
		D3DDISPLAYMODE d3ddm;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		// set the presentation parameters
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		// For storing the quality setting as defined in if statements below
		IScreenRenderTarget::Quality quality;

		// If pScreenRenderTarget != 0 then we have a screen render target that should be used as the default screen render target
		// This is used for fullscreen mode
		if (pScreenRenderTarget != 0)
		{
			quality = pScreenRenderTarget->GetQuality();
			// safe because only DX9FullscreenRenderTargets are assigned to pFullscreenRenderTarget
			d3dpp.BackBufferWidth = pScreenRenderTarget->GetWidth();
			d3dpp.BackBufferHeight = pScreenRenderTarget->GetHeight();
			d3dpp.Windowed = false; // TODO: This value should be read from the pScreenRenderTarget (Which requires it to be a DX9ScreenRenderTarget)
			d3dpp.EnableAutoDepthStencil = pScreenRenderTarget->GetDepthTestEnabled();
		}
		// Create an "empty" default render target
		else
		{
			quality = IScreenRenderTarget::QUALITY_MINIMUM;
			d3dpp.BackBufferWidth = 1; // Default backbuffer should be 1x1 and is never used
			d3dpp.BackBufferHeight = 1; // Default backbuffer should be 1x1 and is never used
			d3dpp.Windowed = true;
			d3dpp.EnableAutoDepthStencil = false; // we manage swap chains manually
		}

		// TODO: need to set back buffer format fist

		CheckMultisampleFormat(pD3D, quality, false);

		d3dpp.BackBufferCount = 1;	// TODO: More than one backbuffer?? What is it good for?
		d3dpp.BackBufferFormat = GetCorrectBackBufferFormat(pD3D); 
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.AutoDepthStencilFormat = GetCorrectDepthStencilFormat(pD3D); 
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.MultiSampleType = FromMultisampleFormat(quality);
		d3dpp.MultiSampleQuality = 0;

		return d3dpp;
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
		else // TODO: will we ever end up here?
			return D3DFMT_X8R8G8B8;
	}


	static D3DFORMAT GetCorrectDepthStencilFormat(IDirect3D9* pD3D)
	{
		// TODO: This similar to the ogre initialization code! Look this over and assert all assumptions!

		D3DDISPLAYMODE displayMode;
		pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		// Get the color depth of the display
		D3DFORMAT displayFormat = GetCorrectBackBufferFormat(pD3D);

		// if we are only running 16 bit color format, then just run a 16 bit depth format...
		if (displayFormat == D3DFMT_R5G6B5)
			return D3DFMT_D16;

		// check support for 24bit depth with 8 bit stencil
		if (SUCCEEDED( pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 )))
		{
			// "nVidia chips since the TNT1 have a restriction that the total depth+stencil bit depth MUST be the same as the bit depth of the frame buffer."
			// - http://www.gamedev.net/community/forums/topic.asp?topic_id=73404
			// TODO: True only for really old cards (order than 2004)?? Check nvida website!

			if( SUCCEEDED( pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayFormat, displayFormat, D3DFMT_D24S8 )))
				return D3DFMT_D24S8;
			else 
				return D3DFMT_D24X8;
		}

		// Ok, so that did not work. Try a pure 32bit depth buffer with no stencil
		if(SUCCEEDED( pD3D->CheckDeviceFormat(D3DFMT_D24S8, D3DDEVTYPE_HAL, displayMode.Format,  D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 )))
			return D3DFMT_D32;

		// If all else failes, run a 16bit buffer
		// TODO: This will probably never happen, but maby we should "crash" here instead??
		return D3DFMT_D16;
	}

	static void CheckMultisampleFormat(IDirect3D9* pD3D, IScreenRenderTarget::Quality& quality, bool fullscreen)
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
			quality = (IScreenRenderTarget::Quality)((int)quality - 1);
		}
	}

	static D3DMULTISAMPLE_TYPE FromMultisampleFormat(IScreenRenderTarget::Quality quality)
	{
		switch(quality)
		{
		case IScreenRenderTarget::QUALITY_MINIMUM:
				return D3DMULTISAMPLE_NONE;
				break;
			case IScreenRenderTarget::QUALITY_LOW:
				return D3DMULTISAMPLE_2_SAMPLES;
				break;
			case IScreenRenderTarget::QUALITY_MEDIUM:
				return D3DMULTISAMPLE_4_SAMPLES;
				break;
			case IScreenRenderTarget::QUALITY_HIGH:
				return D3DMULTISAMPLE_8_SAMPLES;
				break;
			case IScreenRenderTarget::QUALITY_MAXIMUM:
				return D3DMULTISAMPLE_16_SAMPLES;
				break;
		}
		// default case, no multisample
		return D3DMULTISAMPLE_NONE;
	}
};
}

#endif