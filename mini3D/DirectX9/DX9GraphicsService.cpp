#include "DX9GraphicsService.h"

DX9GraphicsService::DX9GraphicsService(const GraphicsSettings& graphicsSettings, int hWindow) :
	pDevice(0), hWindow(hWindow), graphicsSettings(graphicsSettings)
{
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	this->hWindow = hWindow;

	if (pD3D == 0)
	{
		//TODO: Cast some error
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
void DX9GraphicsService::CreateDevice(void)
{
	// get the display mode
	D3DDISPLAYMODE d3ddm;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	// set the presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = 512;
	d3dpp.BackBufferHeight = 512;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		// TODO: Does the device get automatically set to NULL if the above call fails?
		return;
	}
	presentationParameters = d3dpp;
}

void DX9GraphicsService::Release(void)
{
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
	DX9Texture* pDX9Texture = (DX9Texture*)pTexture;

	if (currentWrapStyle != pDX9Texture->GetWrapStyle())
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

		pDevice->SetTextureStageState(index, D3DSAMP_ADDRESSU, adressMode);
		pDevice->SetTextureStageState(index, D3DSAMP_ADDRESSV, adressMode);
		pDevice->SetTextureStageState(index, D3DSAMP_ADDRESSW, adressMode);
	}

	if (pDX9Texture != pCurrentTexture && pDX9Texture->GetTexture() != 0 && pDevice != 0)
	{
		pDevice->SetTexture(index, pDX9Texture->GetTexture());
		pCurrentTexture = pDX9Texture;
		currentWrapStyle = pDX9Texture->GetWrapStyle();
	}
}

ITexture* DX9GraphicsService::GetTexture(unsigned int index)
{
	// Todo: this should be an array
	return pCurrentTexture;
}

void DX9GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{
	DX9RenderTarget* pDX9RenderTarget = (DX9RenderTarget*)pRenderTarget;
	if (pDX9RenderTarget != pCurrentRenderTarget && pDX9RenderTarget->GetRenderTargetBuffer() != 0 && pDevice != 0)
	{
		pDevice->SetRenderTarget(0,  (IDirect3DSurface9*)pDX9RenderTarget->GetRenderTargetBuffer());
		pCurrentRenderTarget = pDX9RenderTarget;
	}
}

IRenderTarget* DX9GraphicsService::GetRenderTarget(void)
{
	return pCurrentRenderTarget;
}

void DX9GraphicsService::SetDepthStencil(IDepthStencil* pDepthStencil)
{
	DX9DepthStencil* pDX9DepthStencil = (DX9DepthStencil*)pDepthStencil;
	if (pDX9DepthStencil != pCurrentDepthStencil && pDX9DepthStencil->GetDepthStencilBuffer() != 0 && pDevice != 0)
	{
		pDevice->SetDepthStencilSurface(pDX9DepthStencil->GetDepthStencilBuffer());
		pCurrentDepthStencil = pDX9DepthStencil;
	}
}

IDepthStencil* DX9GraphicsService::GetDepthStencil(void)
{
	return pCurrentDepthStencil;
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
	pDevice->Present(0,0,0,0);
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
	pDevice->SetVertexDeclaration(pDX9VertexBuffer->GetVertexDeclarationBuffer());
	pCurrentVertexBuffer = pDX9VertexBuffer;
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
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, (DWORD)color, 1.0f, 0);
}

void DX9GraphicsService::ClearDepthStencil(void)
{
	pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}


// Create Resources
IRenderTargetBuffer* DX9GraphicsService::CreateRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow)
{
	return new DX9RenderTargetBuffer(this, width, height, hWindow);
}

IRenderTarget* DX9GraphicsService::CreateRenderTarget(unsigned int width, unsigned int height)
{
	return new DX9RenderTarget(this, width, height);
}

IDepthStencil* DX9GraphicsService::CreateDepthStencil(unsigned int width, unsigned int height)
{
	return new DX9DepthStencil(this, width, height);
}

ITexture* DX9GraphicsService::CreateTexture(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, ITexture::BitDepth bitDepth)
{
	return new DX9Texture(this, pBitmap, width, height, wrapStyle, bitDepth);
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

