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

#include "DX9GraphicsService.h"
#include "support/D3DSetupManager.h"

DX9GraphicsService::DX9GraphicsService(const GraphicsSettings& graphicsSettings, int hWindow) :
	pDevice(0), hWindow(hWindow), graphicsSettings(graphicsSettings), pCurrentDepthStencilBuffer(0)
{

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	pD3D->GetDeviceCaps(0, D3DDEVTYPE_HAL, &deviceCaps);

	currentTextures = new IDirect3DTexture9*[deviceCaps.MaxSimultaneousTextures];
	memset(currentTextures, 0, deviceCaps.MaxSimultaneousTextures * sizeof(IDirect3DTexture9*));

	currentITextures = new ITexture*[deviceCaps.MaxSimultaneousTextures];
	memset(currentITextures, 0, deviceCaps.MaxSimultaneousTextures * sizeof(ITexture*));

	currentWrapStyles = new ITexture::WrapStyle[deviceCaps.MaxSimultaneousTextures];
	memset(currentWrapStyles, ITexture::TILE, deviceCaps.MaxSimultaneousTextures * sizeof(ITexture::WrapStyle));

	this->hWindow = hWindow;

	if (pD3D == 0)
	{
		//TODO: Cast some error??
	}

	CreateDevice();
}

DX9GraphicsService::~DX9GraphicsService(void)
{

}

IDirect3DDevice9* DX9GraphicsService::GetDevice(void)
{
	DX9GraphicsService* gs = this;
	return pDevice;
}


void DX9GraphicsService::UpdateResources(void)
{
	for(ResourceContainer::iterator it = resourceList.begin(); it != resourceList.end(); it++)
	{
		if ((*it)->GetIsDirty() == true)
		{
			(*it)->LoadResource();
		}
	}
}

void DX9GraphicsService::AddResource(IDX9Resource* resource)
{
	resourceList.insert(resource);
}
void DX9GraphicsService::RemoveResource(IDX9Resource* resource)
{
	resourceList.erase(resource);
}
void DX9GraphicsService::PoolVertexDeclaration(const VertexDeclaration& vertexDeclaration)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration);

	// if the declaration is already pooled, just increase the counter
	if (vertexDeclarationPool.find(key) != vertexDeclarationPool.end())
	{
		vertexDeclarationPool[key].first++;
		return;
	}

	// It is not already pooled, we need to create a new one
	int count = vertexDeclaration.GetSizeInBytes() / sizeof(VertexDeclaration::VertexDataType) + 1;
	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[count];
	
	// cumulative offset for the vertexelements
	int offset = 0;
	int textureUsageIndex = 0;
	int uvStream = 0;
	int positionUsageIndex = 0;
	int colorUsageIndex = 0;

	unsigned int sizeInBytes;
	for (int i = 0; i < count; i++)
	{
		// these are the same for all
		pVertexElements[i].Offset = offset;
		pVertexElements[i].Stream = 0;
		
		// set the specific parameters
		switch (vertexDeclaration.GetVertexDataTypes(sizeInBytes)[i])
		{
				
		case vertexDeclaration.POSITION:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT3;
			pVertexElements[i].Usage = D3DDECLUSAGE_POSITION;
			pVertexElements[i].UsageIndex = positionUsageIndex++;
			offset += 12;
			break;
		case vertexDeclaration.COLOR:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_D3DCOLOR;
			pVertexElements[i].Usage = D3DDECLUSAGE_COLOR;
			pVertexElements[i].UsageIndex = colorUsageIndex++;
			offset += 4;
			break;
		case vertexDeclaration.TEXTURECOORDINATE:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT2;
			pVertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pVertexElements[i].UsageIndex = textureUsageIndex++;
			offset += 8;
			break;
		}
	}

	D3DVERTEXELEMENT9 end = D3DDECL_END();
	pVertexElements[count - 1] = end;

	IDirect3DVertexDeclaration9* pVertexDeclaration;
	pDevice->CreateVertexDeclaration(pVertexElements, &pVertexDeclaration);

	// pool the vertex declaration
	vertexDeclarationPool[key] = counterPair(1, pVertexDeclaration);

	delete pVertexElements;
}
void DX9GraphicsService::ReleaseVertexDeclaration(const VertexDeclaration& vertexDeclaration)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration);
	
	if (--vertexDeclarationPool[key].first == 0)
	{
		((IDirect3DVertexDeclaration9*)vertexDeclarationPool[key].second)->Release();
		vertexDeclarationPool.erase(key);
	}

	// if the declaration is already pooled, just increase the counter
	if (vertexDeclarationPool.find(key) != vertexDeclarationPool.end())
	{
		vertexDeclarationPool[key].first++;
		return;
	}

}

std::string DX9GraphicsService::CreateVertexDeclarationKey(const VertexDeclaration& vertexDeclaration)
{
	std::string key = "";
	
	unsigned int size;
	VertexDeclaration::VertexDataType* vertexDataTypes = vertexDeclaration.GetVertexDataTypes(size);
	int count = size / sizeof(VertexDeclaration::VertexDataType);
	for (int i = 0; i < count; i++)
	{
		key += char(vertexDataTypes[i]);
	}
	return key;
}



void DX9GraphicsService::CreateDevice(void)
{
	// get the display mode
	D3DDISPLAYMODE d3ddm;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	// set the presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;

	// TODO: need to set back buffer format fist
	D3DSetupManager::CheckMultisampleFormat(graphicsSettings, pD3D);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = 1; // Default backbuffer should be 1x1 and is never used
	d3dpp.BackBufferHeight = 1; // Default backbuffer should be 1x1 and is never used
	d3dpp.BackBufferCount = 1;	// TODO: More than one backbuffer?? What is it good for?
	d3dpp.BackBufferFormat = D3DSetupManager::GetCorrectBackBufferFormat(graphicsSettings, pD3D); 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = !graphicsSettings.fullscreen;
	d3dpp.EnableAutoDepthStencil = false; // we manage swap chains manually
	d3dpp.AutoDepthStencilFormat = D3DSetupManager::GetCorrectDepthStencilFormat(graphicsSettings, pD3D); 
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DSetupManager::FromMultisampleFormat(graphicsSettings.multisampleFormat);
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

	// store the default back buffer so we can put it back when we reset the device
	pDevice->GetRenderTarget(0, &pDefaultRenderTarget);

}

void DX9GraphicsService::Release(void)
{

	pDevice->SetRenderTarget(0, pDefaultRenderTarget);

	if (pDevice != 0)
		pDevice->Release();

	if (pD3D != 0)
		pD3D->Release();
}

void DX9GraphicsService::SetSettings(const GraphicsSettings& graphicsSettings)
{
	if (this->graphicsSettings == graphicsSettings)
		return;

	this->graphicsSettings = graphicsSettings;
	CreateDevice();
}

GraphicsSettings DX9GraphicsService::GetSettings(void)
{
	return graphicsSettings;
}
D3DPRESENT_PARAMETERS DX9GraphicsService::GetPresentationParameters(void)
{
	return presentationParameters;
}

// States
void DX9GraphicsService::SetPixelShader(IPixelShader* pPixelShader)
{
	DX9PixelShader* pDX9PixelShader = (DX9PixelShader*)pPixelShader;
	if (pDX9PixelShader != pCurrentPixelShader && pDX9PixelShader->GetPixelShaderBuffer() != 0 && pDevice != 0)
	{
		pDevice->SetPixelShader((IDirect3DPixelShader9*)pDX9PixelShader->GetPixelShaderBuffer());
		pCurrentPixelShader = pDX9PixelShader;
	}
}

IPixelShader* DX9GraphicsService::GetPixelShader(void)
{
	return pCurrentPixelShader;
}

void DX9GraphicsService::SetVertexShader(IVertexShader* pVertexShader)
{
	DX9VertexShader* pDX9VertexShader = (DX9VertexShader*)pVertexShader;
	if (pDX9VertexShader != pCurrentVertexShader && pDX9VertexShader->GetVertexShaderBuffer() != 0 && pDevice != 0)
	{
		pDevice->SetVertexShader((IDirect3DVertexShader9*)pDX9VertexShader->GetVertexShaderBuffer());
		pCurrentVertexShader = pDX9VertexShader;
	}
}

IVertexShader* DX9GraphicsService::GetVertexShader(void)
{
	return pCurrentVertexShader;
}

void DX9GraphicsService::SetTexture(ITexture* pTexture, unsigned int index)
{	

	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		// Throw error in debug mode;
		return;
	}

	// this cast is "unfailable" (not exception caught). Whoever inherits from ITexture must also inherit from IDX9Texture
	IDX9Texture* pDX9Texture = dynamic_cast<IDX9Texture*>(pTexture);
	//IDX9Texture* pDX9Texture = (IDX9Texture*)pTexture;

	//TODO: maby support for GetTexture should be removed... then this will not be needed
	currentITextures[index] = pTexture;

	if (pDevice != 0 && pDX9Texture->GetTextureBuffer() != 0 && pDX9Texture->GetTextureBuffer() != currentTextures[index])
	{
		if (currentWrapStyles[index] != pTexture->GetWrapStyle())
		{
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

			pDevice->SetSamplerState(index, D3DSAMP_ADDRESSU, adressMode);
			pDevice->SetSamplerState(index, D3DSAMP_ADDRESSV, adressMode);
			pDevice->SetSamplerState(index, D3DSAMP_ADDRESSW, adressMode);

			currentWrapStyles[index] = pTexture->GetWrapStyle();
		}

		pDevice->SetTexture(index, pDX9Texture->GetTextureBuffer());
		currentTextures[index] = pDX9Texture->GetTextureBuffer();
	}
}

ITexture* DX9GraphicsService::GetTexture(unsigned int index)
{
	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		// Throw error in debug mode;
		return 0;
	}

	return currentITextures[index];
}

void DX9GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{
	if (pDevice == 0)
		return;

	IDX9RenderTarget* pDX9RenderTarget = dynamic_cast<IDX9RenderTarget*>(pRenderTarget);
	if (pDX9RenderTarget != pCurrentRenderTarget && pDX9RenderTarget->GetRenderTargetBuffer() != 0)
	{
		pDevice->SetRenderTarget(0,  (IDirect3DSurface9*)pDX9RenderTarget->GetRenderTargetBuffer());
		pCurrentRenderTarget = pDX9RenderTarget;
	}
	
	if (pRenderTarget->GetDepthTestEnabled() == true)
	{
		SetDepthStencil(pDX9RenderTarget->GetDepthStencil());
	}
}

IDepthStencil* DX9GraphicsService::GetDepthStencil(void)
{
	return 0;
}
void DX9GraphicsService::SetDepthStencil(IDepthStencil* pDepthStencil)
{
	DX9DepthStencil* pDX9DepthStencil = (DX9DepthStencil*)pDepthStencil;

	IDirect3DSurface9* pDepthStencilBuffer;
	if (pDepthStencil != 0)
	{
		pDepthStencilBuffer	= pDX9DepthStencil->GetDepthStencilBuffer();
	}
	else
	{
		pDepthStencilBuffer = 0;
	}

	// clear the current assigned depth stencil if we are setting a rendertarget without depth stencil
	if (pDepthStencilBuffer == 0 && pCurrentDepthStencilBuffer != 0)
	{
		pDevice->SetDepthStencilSurface(0);
		pDevice->SetRenderState(D3DRS_ZENABLE, false);
		pCurrentDepthStencilBuffer = 0;
	}
	else if (pDepthStencilBuffer != 0 && pCurrentDepthStencilBuffer != pDepthStencilBuffer)
	{
		pDevice->SetDepthStencilSurface(pDepthStencilBuffer);
		pDevice->SetRenderState(D3DRS_ZENABLE, true);
		pCurrentDepthStencilBuffer = pDepthStencilBuffer;
	}
}

IRenderTarget* DX9GraphicsService::GetRenderTarget(void)
{
	return pCurrentRenderTarget;
}

// Get Graphics Card Capabilities
int DX9GraphicsService::GetMaxTextures()
{
	return 1;
}

int DX9GraphicsService::GetMaxTextureSize()
{
	return 1024;
}

int DX9GraphicsService::GetPixelShaderVersion()
{
	return 2;
}

int DX9GraphicsService::GetVertexShaderVersion()
{
	return 2;
}


// Drawing Graphics
void DX9GraphicsService::BeginFrame(void)
{
	if (pDevice == 0)
		return;
	
	UpdateResources();

	// Set Render States
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //D3DCULL_CCW
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));

	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//pDevice->SetRenderState(D3DRS_CLIPPING, true);

	//pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// Store the default Swap Chain
	//TODO: Default Swapchain
}

// before frame is started, resources will be updated
void DX9GraphicsService::EndFrame(void)
{

}

void DX9GraphicsService::BeginDraw(void)
{
	// TODO: pDevice->BeginStateBlock(); ???
	pDevice->BeginScene();
}

void DX9GraphicsService::EndDraw(void)
{
	pDevice->EndScene();
}


void DX9GraphicsService::SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantF(index, pData, count);
}

void DX9GraphicsService::SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantI(index, pData, count);
}

void DX9GraphicsService::SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count)
{
	pDevice->SetVertexShaderConstantB(index, (BOOL*)pData, count);
}

IIndexBuffer* DX9GraphicsService::GetIndexBuffer(void)
{	
	return 0;
}

void DX9GraphicsService::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
{
	DX9IndexBuffer* pDX9IndexBuffer = (DX9IndexBuffer*)pIndexBuffer;
	pDevice->SetIndices(pDX9IndexBuffer->GetIndexBuffer());
	pCurrentIndexBuffer = pDX9IndexBuffer;
}

IVertexBuffer* DX9GraphicsService::GetVertexBuffer(void)
{
	return 0;
}

void DX9GraphicsService::SetVertexBuffer(IVertexBuffer* pVertexBuffer)
{
	DX9VertexBuffer* pDX9VertexBuffer = (DX9VertexBuffer*)pVertexBuffer;
	pDevice->SetStreamSource(0, pDX9VertexBuffer->GetVertexBuffer(), 0, pDX9VertexBuffer->GetVertexSizeInBytes());
	pCurrentVertexBuffer = pDX9VertexBuffer;
	
	IDirect3DVertexDeclaration9* pVD = vertexDeclarationPool[CreateVertexDeclarationKey(pDX9VertexBuffer->GetVertexDeclaration())].second;
	pDevice->SetVertexDeclaration(pVD);

}


void DX9GraphicsService::Draw(void)
{
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), 0, pCurrentIndexBuffer->GetIndexCount() / 3);
}

void DX9GraphicsService::DrawIndices(unsigned int startIndex, unsigned int numIndices)
{
	DX9VertexBuffer* pDX9VertexBuffer = (DX9VertexBuffer*)pCurrentVertexBuffer;
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), startIndex, numIndices);
}


void DX9GraphicsService::ClearRenderTarget(int color)
{

	DWORD flags = D3DCLEAR_TARGET;
	
	// if we have a depthstencil we need to clear that too
	if (pCurrentDepthStencilBuffer != 0)
	{
		flags |= D3DCLEAR_ZBUFFER;
	}
	pDevice->Clear(0, 0, flags, (DWORD)color, 1.0f, 0);
}


// Create Resources
IScreenRenderTarget* DX9GraphicsService::CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled)
{
	return new DX9ScreenRenderTarget(this, width, height, hWindow, depthTestEnabled);
}

IRenderTargetTexture* DX9GraphicsService::CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled)
{
	return new DX9RenderTargetTexture(this, width, height, depthTestEnabled);
}

IBitmapTexture* DX9GraphicsService::CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, IBitmapTexture::BitDepth bitDepth)
{
	return new DX9BitmapTexture(this, pBitmap, width, height, wrapStyle, bitDepth);
}

IVertexBuffer* DX9GraphicsService::CreateVertexBuffer(void* vertices, unsigned int count, const VertexDeclaration& vertexDeclaration)
{
	return new DX9VertexBuffer(this, vertices, count, vertexDeclaration);
}

IIndexBuffer* DX9GraphicsService::CreateIndexBuffer(int* indices, unsigned int count)
{
	return new DX9IndexBuffer(this, indices, count);
}

IPixelShader* DX9GraphicsService::CreatePixelShader(char* shaderBytes, unsigned int sizeInBytes)
{
	return new DX9PixelShader(this, shaderBytes, sizeInBytes);
}

IVertexShader* DX9GraphicsService::CreateVertexShader(char* shaderBytes, unsigned int sizeInBytes)
{
	return new DX9VertexShader(this, shaderBytes, sizeInBytes);
}

