#include "D3DSetupManager.h"

D3DFORMAT D3DSetupManager::GetCorrectBackBufferFormat(GraphicsSettings& graphicsSettings, IDirect3D9* pD3D)
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


D3DFORMAT D3DSetupManager::GetCorrectDepthStencilFormat(GraphicsSettings& graphicsSettings, IDirect3D9* pD3D)
{
	// TODO: This similar to the ogre initialization code! Look this over and assert all assumptions!

	D3DDISPLAYMODE displayMode;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	// Get the color depth of the display
	D3DFORMAT displayFormat = GetCorrectBackBufferFormat(graphicsSettings, pD3D);

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

void D3DSetupManager::CheckMultisampleFormat(GraphicsSettings& graphicsSettings, IDirect3D9* pD3D)
{

	D3DDISPLAYMODE displayMode;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	

	DWORD pQualityLevels;
	while (	graphicsSettings.multisampleFormat > 0 && 
			FAILED(pD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
														D3DDEVTYPE_HAL, 
														displayMode.Format, 
														!graphicsSettings.fullscreen,
														FromMultisampleFormat(graphicsSettings.multisampleFormat),
														&pQualityLevels)))
	{
		graphicsSettings.multisampleFormat = (GraphicsSettings::MultisampleFormat)((int)graphicsSettings.multisampleFormat - 1);
	}
}

D3DMULTISAMPLE_TYPE D3DSetupManager::FromMultisampleFormat(GraphicsSettings::MultisampleFormat multisampleFormat)
{
	switch(multisampleFormat)
	{
		case GraphicsSettings::NO_MULTISAMPLE:
			return D3DMULTISAMPLE_NONE;
			break;
		case GraphicsSettings::TWO_SAMPLES:
			return D3DMULTISAMPLE_2_SAMPLES;
			break;
		case GraphicsSettings::FOUR_SAMPLES:
			return D3DMULTISAMPLE_4_SAMPLES;
			break;
		case GraphicsSettings::EIGHT_SAMPLES:
			return D3DMULTISAMPLE_8_SAMPLES;
			break;
		case GraphicsSettings::SIXTEEN_SAMPLES:
			return D3DMULTISAMPLE_16_SAMPLES;
			break;
	}
	// default case, no multisample
	return D3DMULTISAMPLE_NONE;
}
