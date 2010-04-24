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

#include "../DX9GraphicsService.h"


// Constructor Destructor -----------------------------------------------------

mini3d::DX9GraphicsService::DX9GraphicsService(int hWindow) :
	pDevice(0), hWindow(hWindow), pCurrentDepthStencil(0), isDrawingScene(false), deviceLost(true), pCurrentRenderTargetBuffer(0)
{
	CreateDevice();
}
mini3d::DX9GraphicsService::~DX9GraphicsService(void)
{

	UnloadResources();

	if (pDefaultRenderTarget != 0)
		pDefaultRenderTarget->Release();

	if (pDevice != 0)
		pDevice->Release();

	if (pD3D != 0)
		pD3D->Release();
}



// Friend Functions -----------------------------------------------------------

IDirect3DDevice9* mini3d::DX9GraphicsService::GetDevice(void)
{
	DX9GraphicsService* gs = this;
	return pDevice;
}
D3DPRESENT_PARAMETERS mini3d::DX9GraphicsService::GetPresentationParameters(void)
{
	return presentationParameters;
}



// Private helper methods -----------------------------------------------------

void mini3d::DX9GraphicsService::CreateDevice(void)
{
	pD3D=Direct3DCreate9(D3D_SDK_VERSION);
	pD3D->GetDeviceCaps(0, D3DDEVTYPE_HAL, &deviceCaps);

	currentITextures = new ITexture*[deviceCaps.MaxSimultaneousTextures];
	lostDeviceCurrentITextures = new ITexture*[deviceCaps.MaxSimultaneousTextures];
	memset(currentITextures, 0, deviceCaps.MaxSimultaneousTextures * sizeof(ITexture*));

	if (pD3D == 0)
	{
		//TODO: Cast some error??
	}
	
	// get the display mode
	D3DDISPLAYMODE d3ddm;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	// set the presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;

	// TODO: need to set back buffer format fist
	IScreenRenderTarget::Quality quality = IScreenRenderTarget::QUALITY_MINIMUM;
	CheckMultisampleFormat(quality, false);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = 1; // Default backbuffer should be 1x1 and is never used
	d3dpp.BackBufferHeight = 1; // Default backbuffer should be 1x1 and is never used
	d3dpp.BackBufferCount = 1;	// TODO: More than one backbuffer?? What is it good for?
	d3dpp.BackBufferFormat = GetCorrectBackBufferFormat(); 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = false; // we manage swap chains manually
	d3dpp.AutoDepthStencilFormat = GetCorrectDepthStencilFormat(); 
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = FromMultisampleFormat(quality);
	d3dpp.MultiSampleQuality = 0;

	presentationParameters = d3dpp;

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &pDevice)))
		{
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
			{				
				// TODO: ALL FAILED THROW ERROR!!
				throw;
			}
		}
	}

	// Device created correctly, set device lost to false
	deviceLost = false;

	// store the default back buffer so we can put it back when we reset the device
	pDevice->GetRenderTarget(0, &pDefaultRenderTarget);
}

// Resource Management
void mini3d::DX9GraphicsService::UpdateResources(void)
{
	// update resources
	for(ResourceContainer::iterator it = resourceList.begin(); it != resourceList.end(); it++)
	{
		if ((*it)->GetIsDirty() == true)
		{
			(*it)->LoadResource();
		}
	}
}
void mini3d::DX9GraphicsService::UnloadResources(void)
{
	// update resources
	for(ResourceContainer::iterator it = resourceList.begin(); it != resourceList.end(); it++)
	{
		(*it)->UnloadResource();
	}
}

void mini3d::DX9GraphicsService::AddResource(IDX9Resource* resource)
{
	resourceList.insert(resource);
}
void mini3d::DX9GraphicsService::RemoveResource(IDX9Resource* resource)
{
	resourceList.erase(resource);
}

void mini3d::DX9GraphicsService::SaveGraphicsState(void)
{
	pLostDeviceRenderTarget = pCurrentRenderTarget;
	pLostDeviceDepthStencil = pCurrentDepthStencil;
	pLostDeviceVertexBuffer = pCurrentVertexBuffer;
	pLostDeviceIndexBuffer = pCurrentIndexBuffer;
	pLostDevicePixelShader = pCurrentPixelShader;
	pLostDeviceVertexShader = pCurrentVertexShader;

	for (int i = 0; i < GetMaxTextures(); i++)
		lostDeviceCurrentITextures[i] = currentITextures[i];

}
void mini3d::DX9GraphicsService::RestoreGraphicsState(void)
{
	SetRenderTarget(pLostDeviceRenderTarget);
	SetDepthStencil(pLostDeviceDepthStencil);
	SetVertexBuffer(pLostDeviceVertexBuffer);
	SetIndexBuffer(pLostDeviceIndexBuffer);
	SetPixelShader(pLostDevicePixelShader);
	SetVertexShader(pLostDeviceVertexShader);
	for (int i = 0; i < GetMaxTextures(); i++)
		SetTexture(lostDeviceCurrentITextures[i], i);

}

void mini3d::DX9GraphicsService::HandleLostDevice()
{
	// find lost device type
	int deviceState = pDevice->TestCooperativeLevel();

	// if it is a driver internal error, just recreate the whole thing and return
	if (deviceState == D3DERR_DRIVERINTERNALERROR)
	{
		// Store away the current state of the graphics pipeline
		SaveGraphicsState();

		// Completely tear down the device
		UnloadResources();

		// Release the default render target 
		if (pDefaultRenderTarget != 0)
		{
			// This must be done after UnloadResources since UnloadResources restores
			// the default render target
			pDefaultRenderTarget->Release();
			pDefaultRenderTarget = 0;
		}

		pDevice->Release();
		pD3D->Release();

		// try to recreate the device
		// this will throw an error if the driver is broken so there is no need to do that explicitly
		CreateDevice();
		UpdateResources();

		// put back the stored graphics pipeline states
		RestoreGraphicsState();

		return;
	}

	// check if this is the first time we get the lost device
	if (deviceLost == false)
	{
		deviceLost = true;

		// Store away the current state of the graphics pipeline
		SaveGraphicsState();

		// unload all resources
		UnloadResources();

		// Release the default render target 
		if (pDefaultRenderTarget != 0)
		{
			// This must be done after UnloadResources since UnloadResources restores
			// the default render target
			pDefaultRenderTarget->Release();
			pDefaultRenderTarget = 0;
		}
	}
	
	// se if we can recreate the device
	if (deviceState == D3DERR_DEVICENOTRESET)
	{
		int newDeviceState = pDevice->Reset(&presentationParameters);
		
		// if it was not successful, just return and try agian later
		if (newDeviceState != D3D_OK)
			return;

		// capture the new default render target
		pDevice->GetRenderTarget(0, &pDefaultRenderTarget);

		//if it was succesful, reload all resources and set device lost to false
		UpdateResources();

		// put back the current graphics pipeline states
		RestoreGraphicsState();

		deviceLost = false;
	}

	// device is lost but can not yet be recreated.
	if (deviceState == D3DERR_DEVICELOST)
	{
		// do nothing
	}
}


// Vertex Declaration Pool
void mini3d::DX9GraphicsService::PoolVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration);

	// if the declaration is already pooled, just increase the counter
	if (vertexDeclarationPool.find(key) != vertexDeclarationPool.end())
	{
		vertexDeclarationPool[key].counter++;
		return;
	}

	// create a direct3d 9 vertex delcaration from the IVertexShader vertex declaration
	IDirect3DVertexDeclaration9* pVertexDeclaration = CreateDX9VertexDeclaration(vertexDeclaration);

	// pool the vertex declaration
	vertexDeclarationPool[key] = VertexDeclarationContainer(vertexDeclaration, pVertexDeclaration, 1);
}
void mini3d::DX9GraphicsService::ReleaseVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration);
	
	if (--vertexDeclarationPool[key].counter == 0)
	{
		vertexDeclarationPool[key].direct3dDeclaration->Release();
		vertexDeclarationPool.erase(key);
	}

	// if the declaration is already pooled, just increase the counter
	if (vertexDeclarationPool.find(key) != vertexDeclarationPool.end())
	{
		vertexDeclarationPool[key].counter++;
		return;
	}

}

// Creates vertex declaration pool key from vertex declaration
std::string mini3d::DX9GraphicsService::CreateVertexDeclarationKey(const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	std::string key = "";
	
	for (unsigned int i = 0; i < vertexDeclaration.size(); i++)
	{
		key += char(vertexDeclaration[i]);
	}
	return key;
}
IDirect3DVertexDeclaration9* mini3d::DX9GraphicsService::CreateDX9VertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	// It is not already pooled, we need to create a new one
	int count = vertexDeclaration.size();
	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[count + 1];
	
	// cumulative offset for the vertexelements
	int offset = 0;
	int textureUsageIndex = 0;
	int uvStream = 0;
	int positionUsageIndex = 0;
	int colorUsageIndex = 0;

	for (int i = 0; i < count; i++)
	{
		// these are the same for all
		pVertexElements[i].Offset = offset;
		pVertexElements[i].Stream = 0;
		
		// set the specific parameters
		switch (vertexDeclaration[i])
		{
				
		case IVertexShader::POSITION:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT3;
			pVertexElements[i].Usage = D3DDECLUSAGE_POSITION;
			pVertexElements[i].UsageIndex = positionUsageIndex++;
			offset += 12;
			break;
		case IVertexShader::COLOR:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_D3DCOLOR;
			pVertexElements[i].Usage = D3DDECLUSAGE_COLOR;
			pVertexElements[i].UsageIndex = colorUsageIndex++;
			offset += 4;
			break;
		case IVertexShader::TEXTURECOORDINATE:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT2;
			pVertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pVertexElements[i].UsageIndex = textureUsageIndex++;
			offset += 8;
			break;
		}
	}

	D3DVERTEXELEMENT9 end = D3DDECL_END();
	pVertexElements[count] = end;

	IDirect3DVertexDeclaration9* pVertexDeclaration;
	pDevice->CreateVertexDeclaration(pVertexElements, &pVertexDeclaration);

	delete pVertexElements;

	return pVertexDeclaration;
}


// Locking resources
void mini3d::DX9GraphicsService::BeginScene(void)
{
	if (isDrawingScene == true)
		return;

	if (pDevice == 0)
		return;

	// check for lost device
	if (deviceLost || pDevice->TestCooperativeLevel() != D3D_OK)
	{
		HandleLostDevice();

		//if device is still lost, just return
		if (deviceLost == true)
			return;
	}

	// Todo: Move this to device reset and update resources immidiatly
	UpdateResources();

	pDevice->BeginScene();

	isDrawingScene = true;
}
void mini3d::DX9GraphicsService::EndScene(void)
{
	pDevice->EndScene();
	isDrawingScene = false;
}

// setting render states
void mini3d::DX9GraphicsService::SetRenderStates()
{
	// Set Render States
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //D3DCULL_CCW
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));

	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_CLIPPING, true);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
}



// Graphics Pipeline States ---------------------------------------------------

int mini3d::DX9GraphicsService::GetMaxTextures()
{
	return deviceCaps.MaxSimultaneousTextures;
}
int mini3d::DX9GraphicsService::GetMaxTextureSize()
{
	return min(deviceCaps.MaxTextureHeight, deviceCaps.MaxTextureWidth);
}
int mini3d::DX9GraphicsService::GetPixelShaderVersion()
{
	return deviceCaps.PixelShaderVersion;
}
int mini3d::DX9GraphicsService::GetVertexShaderVersion()
{
	return deviceCaps.VertexShaderVersion;
}



// Graphics Pipeline States ---------------------------------------------------

// Pixlel Shader
mini3d::IPixelShader* mini3d::DX9GraphicsService::GetPixelShader(void)
{
	return pCurrentPixelShader;
}
void mini3d::DX9GraphicsService::SetPixelShader(IPixelShader* pPixelShader)
{
	if (pCurrentPixelShader == pPixelShader)
		return;

	if (pPixelShader == 0)
	{
		pDevice->SetPixelShader(0);
	}
	else
	{
		DX9PixelShader* pDX9PixelShader = (DX9PixelShader*)pPixelShader;
		pDevice->SetPixelShader((IDirect3DPixelShader9*)pDX9PixelShader->GetPixelShaderBuffer());
	}

	pCurrentPixelShader = pPixelShader;
}

// Vertex Shader
mini3d::IVertexShader* mini3d::DX9GraphicsService::GetVertexShader(void)
{
	return pCurrentVertexShader;
}
void mini3d::DX9GraphicsService::SetVertexShader(IVertexShader* pVertexShader)
{
	if (pCurrentVertexShader == pVertexShader)
		return;

	if (pVertexShader == 0)
	{
		pDevice->SetVertexShader(0);
		pDevice->SetVertexDeclaration(0);
	}
	else
	{
		DX9VertexShader* pDX9VertexShader = (DX9VertexShader*)pVertexShader;
		pDevice->SetVertexShader((IDirect3DVertexShader9*)pDX9VertexShader->GetVertexShaderBuffer());

		std::string key = CreateVertexDeclarationKey(pDX9VertexShader->GetVertexDeclaration());
		pDevice->SetVertexDeclaration(vertexDeclarationPool[key].direct3dDeclaration);
	}

	pCurrentVertexShader = pVertexShader;

}

// Texture
mini3d::ITexture* mini3d::DX9GraphicsService::GetTexture(unsigned int index)
{
	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		// Throw error in debug mode;
		return 0;
	}

	return currentITextures[index];
}
void mini3d::DX9GraphicsService::SetTexture(ITexture* pTexture, unsigned int index)
{	

	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		// TODO: Throw error in debug mode;
		return;
	}

	// if texture already assigned, then there is no need to re-assign it
	if (pTexture == currentITextures[index])
		return;

	if (pTexture == 0)
	{
		pDevice->SetTexture(index, 0);
	}
	else
	{

		// this cast is "unfailable" (not exception caught). Whoever inherits from ITexture must also inherit from IDX9Texture
		IDX9Texture* pDX9Texture = dynamic_cast<IDX9Texture*>(pTexture);

		// we always set the wrapstyle too... if we are changing the texture the overhead of also changing wrap style is small.
		D3DTEXTUREADDRESS adressMode;

		switch(pDX9Texture->GetWrapStyle())
		{
		case ITexture::TILE:
			adressMode = D3DTADDRESS_WRAP;
			break;
		case ITexture::MIRROR:
			adressMode = D3DTADDRESS_MIRROR;
			break;
		case ITexture::CLAMP:
			adressMode = D3DTADDRESS_CLAMP;
			break;
		}

		// set the wrap style
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSU, adressMode);
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSV, adressMode);
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSW, adressMode);
	
		// set the texture
		pDevice->SetTexture(index, pDX9Texture->GetTextureBuffer());
	}
	currentITextures[index] = pTexture;
}

// Render Target
mini3d::IRenderTarget* mini3d::DX9GraphicsService::GetRenderTarget(void)
{
	return pCurrentRenderTarget;
}
void mini3d::DX9GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{
	if (pRenderTarget == pCurrentRenderTarget)
		return;

	// release the old render target!
	if (pCurrentRenderTargetBuffer != 0)
	{
		pCurrentRenderTargetBuffer->Release();
	}

	if (pRenderTarget == 0)
	{
		pDevice->SetRenderTarget(0,  pDefaultRenderTarget);
		SetDepthStencil(0);
		pCurrentRenderTargetBuffer = 0;
	}
	else
	{
		// this cast is "unfailable" (not exception caught). Whoever inherits from IRenderTarget must also inherit from IDX9RenderTarget
		IDX9RenderTarget* pDX9RenderTarget = dynamic_cast<IDX9RenderTarget*>(pRenderTarget);
	
		IDirect3DSurface9* pRenderTargetBuffer = (IDirect3DSurface9*)pDX9RenderTarget->GetRenderTargetBuffer();
		pDevice->SetRenderTarget(0, pRenderTargetBuffer);
	
		if (pRenderTarget->GetDepthTestEnabled() == true)
			SetDepthStencil(pDX9RenderTarget->GetDepthStencil());

		pCurrentRenderTargetBuffer = pRenderTargetBuffer; 
	}

	pCurrentRenderTarget = pRenderTarget;

}

// DepthStencil
mini3d::IDepthStencil* mini3d::DX9GraphicsService::GetDepthStencil(void)
{
	return pCurrentDepthStencil;
}
void mini3d::DX9GraphicsService::SetDepthStencil(IDepthStencil* pDepthStencil)
{
	if (pDepthStencil == pCurrentDepthStencil)
		return;
	
	DX9DepthStencil* pDX9DepthStencil = (DX9DepthStencil*)pDepthStencil;
	
	if (pDepthStencil == 0)
	{
		pDevice->SetDepthStencilSurface(0);
		pDevice->SetRenderState(D3DRS_ZENABLE, true);
	}
	else
	{
		pDevice->SetDepthStencilSurface(pDX9DepthStencil->GetDepthStencilBuffer());
		pDevice->SetRenderState(D3DRS_ZENABLE, true);
	}

	pCurrentDepthStencil = pDepthStencil;
}

// Index Buffer
mini3d::IIndexBuffer* mini3d::DX9GraphicsService::GetIndexBuffer(void)
{	
	return pCurrentIndexBuffer;
}
void mini3d::DX9GraphicsService::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
{
	if (pCurrentIndexBuffer == pIndexBuffer)
		return;

	if (pIndexBuffer == 0)
	{
		pDevice->SetIndices(0);
	}
	else
	{
		DX9IndexBuffer* pDX9IndexBuffer = (DX9IndexBuffer*)pIndexBuffer;
		pDevice->SetIndices(pDX9IndexBuffer->GetIndexBuffer());
	}

	pCurrentIndexBuffer = pIndexBuffer;
}

// Vertex Buffer
mini3d::IVertexBuffer* mini3d::DX9GraphicsService::GetVertexBuffer(void)
{
	return pCurrentVertexBuffer;
}
void mini3d::DX9GraphicsService::SetVertexBuffer(IVertexBuffer* pVertexBuffer)
{
	if (pCurrentVertexBuffer == pVertexBuffer)
		return;

	if (pVertexBuffer == 0)
	{
		pDevice->SetStreamSource(0, 0, 0, 0);
	}
	else
	{
		DX9VertexBuffer* pDX9VertexBuffer = (DX9VertexBuffer*)pVertexBuffer;
		pDevice->SetStreamSource(0, pDX9VertexBuffer->GetVertexBuffer(), 0, pDX9VertexBuffer->GetVertexSizeInBytes());
	}

	pCurrentVertexBuffer = pVertexBuffer;
}

// Shader Parameters
void mini3d::DX9GraphicsService::SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantF(index, pData, count);
}
void mini3d::DX9GraphicsService::SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantI(index, pData, count);
}
void mini3d::DX9GraphicsService::SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantB(index, (BOOL*)pData, count);
}


// Drawing
void mini3d::DX9GraphicsService::Draw(void)
{
	BeginScene();
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), 0, pCurrentIndexBuffer->GetIndexCount() / 3);
}
void mini3d::DX9GraphicsService::DrawIndices(unsigned int startIndex, unsigned int numIndices)
{
	BeginScene();
	DX9VertexBuffer* pDX9VertexBuffer = (DX9VertexBuffer*)pCurrentVertexBuffer;
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), startIndex, numIndices);
}

// Clear
void mini3d::DX9GraphicsService::Clear(int color)
{

	DWORD flags = D3DCLEAR_TARGET;
	
	// if we have a depthstencil we need to clear that too
	if (pCurrentDepthStencil != 0)
	{
		flags |= D3DCLEAR_ZBUFFER;
	}
	pDevice->Clear(0, 0, flags, (DWORD)color, 1.0f, 0);
}


// Create Resources
mini3d::IScreenRenderTarget* mini3d::DX9GraphicsService::CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IScreenRenderTarget::Quality quality)
{
	return new DX9ScreenRenderTarget(this, width, height, hWindow, depthTestEnabled, quality);
}
mini3d::IRenderTargetTexture* mini3d::DX9GraphicsService::CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled)
{
	return new DX9RenderTargetTexture(this, width, height, depthTestEnabled);
}
mini3d::IBitmapTexture* mini3d::DX9GraphicsService::CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth, ITexture::WrapStyle wrapStyle)
{
	return new DX9BitmapTexture(this, pBitmap, width, height, bitDepth, wrapStyle);
}
mini3d::IVertexBuffer* mini3d::DX9GraphicsService::CreateVertexBuffer(void* vertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration)
{
	return new DX9VertexBuffer(this, vertices, count, vertexDeclaration);
}
mini3d::IIndexBuffer* mini3d::DX9GraphicsService::CreateIndexBuffer(int* indices, unsigned int count, const IIndexBuffer::CullMode cullMode)
{
	return new DX9IndexBuffer(this, indices, count);
}
mini3d::IPixelShader* mini3d::DX9GraphicsService::CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes)
{
	return new DX9PixelShader(this, shaderBytes);
}
mini3d::IVertexShader* mini3d::DX9GraphicsService::CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	return new DX9VertexShader(this, shaderBytes, vertexDeclaration);
}


// Set up helper functions

D3DFORMAT mini3d::DX9GraphicsService::GetCorrectBackBufferFormat(void)
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


D3DFORMAT mini3d::DX9GraphicsService::GetCorrectDepthStencilFormat(void)
{
	// TODO: This similar to the ogre initialization code! Look this over and assert all assumptions!

	D3DDISPLAYMODE displayMode;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	// Get the color depth of the display
	D3DFORMAT displayFormat = GetCorrectBackBufferFormat();

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

void mini3d::DX9GraphicsService::CheckMultisampleFormat(IScreenRenderTarget::Quality& quality, bool fullscreen)
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

D3DMULTISAMPLE_TYPE mini3d::DX9GraphicsService::FromMultisampleFormat(IScreenRenderTarget::Quality quality)
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
