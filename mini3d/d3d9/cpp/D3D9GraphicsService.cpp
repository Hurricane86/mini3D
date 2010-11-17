
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../D3D9GraphicsService.h"
#include "../D3D9PresentationParameters.h"
#include "../D3D9GraphicsSettings.h"
#include "../../error/error.h"


// Constructor Destructor -----------------------------------------------------

mini3d::D3D9GraphicsService::D3D9GraphicsService() :
pD3D(0), pDevice(0), isDrawingScene(false), deviceLost(true), lostDeviceCurrentITextures(0), currentITextures(0), isFullscreen(false), pCurrentRenderTarget(0), pDefaultSwapChain(0) 
{
	Init();
	CreateDevice();
}

mini3d::D3D9GraphicsService::~D3D9GraphicsService()
{
	TearDownDevice();
	Dispose();
}

void mini3d::D3D9GraphicsService::Init()
{
	if (pD3D == 0)
		pD3D=Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3D == 0)
	{
		throw Error::MINI3D_ERROR_UNKNOWN;
	}

	// Set the texture arrays to correct size
	pD3D->GetDeviceCaps(0, D3DDEVTYPE_HAL, &deviceCaps);

	if (lostDeviceCurrentITextures != 0)
		delete[] lostDeviceCurrentITextures;

	lostDeviceCurrentITextures = new ITexture*[deviceCaps.MaxSimultaneousTextures];

	if (currentITextures != 0)
		delete[] currentITextures;

	currentITextures = new ITexture*[deviceCaps.MaxSimultaneousTextures];
	memset(currentITextures, 0, deviceCaps.MaxSimultaneousTextures * sizeof(ITexture*));

	// Create the window to hold the default render target
	CreateInternalWindow();
}

void mini3d::D3D9GraphicsService::Dispose()
{
	if (lostDeviceCurrentITextures != 0)
	{
		delete[] lostDeviceCurrentITextures;
		lostDeviceCurrentITextures = 0;
	}

	if (currentITextures != 0)
	{
		delete[] currentITextures;
		lostDeviceCurrentITextures = 0;
	}

	DisposeInternalWindow();

	if (pD3D != 0)
		pD3D->Release();
}

// Friend Functions -----------------------------------------------------------

IDirect3DDevice9* mini3d::D3D9GraphicsService::GetDevice()
{
	D3D9GraphicsService* gs = this;
	return pDevice;
}
D3DPRESENT_PARAMETERS mini3d::D3D9GraphicsService::GetPresentationParameters()
{
	return D3D9PresentationParameters::GetPresentationParametersFromGraphicsSettings(pD3D, pDefaultSwapChain);
}



// Private helper methods -----------------------------------------------------


void mini3d::D3D9GraphicsService::CreateInternalWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = DefWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"D3D9InternalWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
		hWindow = 0;
		return;
    }

	hWindow = CreateWindowEx(WS_EX_CLIENTEDGE, L"D3D9InternalWindowClass", L"HiddenWindow", 0, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, HWND_MESSAGE, 0, hInstance, 0);
}

void mini3d::D3D9GraphicsService::DisposeInternalWindow()
{
	if (hWindow != 0)
	{
		DestroyWindow(hWindow);
		hWindow = 0;
	}
}


// Resource Management
void mini3d::D3D9GraphicsService::UpdateResources()
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
void mini3d::D3D9GraphicsService::UnloadResources()
{
	// update resources
	for(ResourceContainer::iterator it = resourceList.begin(); it != resourceList.end(); it++)
	{
		(*it)->UnloadResource();
	}
}

void mini3d::D3D9GraphicsService::AddResource(ID3D9Resource* resource)
{
	resourceList.insert(resource);
}
void mini3d::D3D9GraphicsService::RemoveResource(ID3D9Resource* resource)
{
	resourceList.erase(resource);
}

void mini3d::D3D9GraphicsService::SaveGraphicsState()
{
	pLostDeviceRenderTarget = pCurrentRenderTarget; 
	pCurrentRenderTarget = 0;

	pLostDeviceVertexBuffer = pCurrentVertexBuffer; 
	pCurrentVertexBuffer = 0;

	pLostDeviceIndexBuffer = pCurrentIndexBuffer; 
	pCurrentIndexBuffer = 0;

	pLostDevicePixelShader = pCurrentPixelShader; 
	pCurrentPixelShader = 0;

	pLostDeviceVertexShader = pCurrentVertexShader; 
	pCurrentVertexShader = 0;

	pLostDeviceShaderProgram = pCurrentShaderProgram;

	for (unsigned int i = 0; i < GetMaxTextures(); i++)
	{
		lostDeviceCurrentITextures[i] = currentITextures[i]; 
		currentITextures[i] = 0;
	}

}
void mini3d::D3D9GraphicsService::RestoreGraphicsState()
{
	SetRenderTarget(pLostDeviceRenderTarget);
	SetVertexBuffer(pLostDeviceVertexBuffer);
	SetIndexBuffer(pLostDeviceIndexBuffer);
	SetPixelShader(pLostDevicePixelShader);
	SetVertexShader(pLostDeviceVertexShader);
	SetShaderProgram(pLostDeviceShaderProgram);
	for (unsigned int i = 0; i < GetMaxTextures(); i++)
		SetTexture(lostDeviceCurrentITextures[i], i);

}

void mini3d::D3D9GraphicsService::RecreateDevice()
{
	// Tear down the old device
	TearDownDevice();

	// try to recreate the device
	// this will throw an error if the driver is broken so there is no need to do that explicitly
	CreateDevice();

	// Reload the resources
	UpdateResources();

	// put back the stored graphics pipeline states
	RestoreGraphicsState();
}

void mini3d::D3D9GraphicsService::CreateDevice()
{
	
	// if we are not in fullscreen mode then pDefaultSwapChain == 0
	D3DPRESENT_PARAMETERS d3dpp = D3D9PresentationParameters::GetPresentationParametersFromGraphicsSettings(pD3D, pDefaultSwapChain);
	presentationParameters = d3dpp;

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &pDevice)))
		{
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
			{				
				throw Error::MINI3D_ERROR_UNKNOWN;
			}
		}
	}

	// pDefaultSwapChain is 0 if we are not in fullscreen mode
	pCurrentRenderTarget = pDefaultSwapChain;
	
	// Device created correctly, set device lost to false
	deviceLost = false;

	// store the default back buffer so we can put it back when we reset the device
	pDevice->GetRenderTarget(0, &pDefaultRenderTarget);

	// get the depthstencil even if it might be 0
	pDevice->GetDepthStencilSurface(&pDefaultDepthStencilSurface);
}

void mini3d::D3D9GraphicsService::TearDownDevice()
{
	ReleaseDevice();

	pDevice->Release();
	pDevice = 0;
}


void mini3d::D3D9GraphicsService::ResetDevice()
{
	ReleaseDevice();

	D3DPRESENT_PARAMETERS d3dpp = D3D9PresentationParameters::GetPresentationParametersFromGraphicsSettings(pD3D, pDefaultSwapChain);
	presentationParameters = d3dpp;

	RestoreDevice();
}

void mini3d::D3D9GraphicsService::RestoreDevice()
{
	D3DPRESENT_PARAMETERS pp = D3D9PresentationParameters::GetPresentationParametersFromGraphicsSettings(pD3D, pDefaultSwapChain);
	int newDeviceState = pDevice->Reset(&pp);
		
	// if it was not successful, just return and try agian later
	if (newDeviceState != D3D_OK)
		return;

	// capture the new default render target
	pDevice->GetRenderTarget(0, &pDefaultRenderTarget);
	pDevice->GetDepthStencilSurface(&pDefaultDepthStencilSurface);

	//if it was succesful, reload all resources and set device lost to false
	UpdateResources();

	// put back the current graphics pipeline states
	RestoreGraphicsState();

	deviceLost = false;
}

void mini3d::D3D9GraphicsService::ReleaseDevice()
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

	if (pDefaultDepthStencilSurface != 0)
	{
		pDefaultDepthStencilSurface->Release();
		pDefaultDepthStencilSurface = 0;
	}
}


void mini3d::D3D9GraphicsService::HandleLostDevice()
{
	// find lost device type
	int deviceState = pDevice->TestCooperativeLevel();

	// if it is a driver internal error, just recreate the whole thing and return
	if (deviceState == D3DERR_DRIVERINTERNALERROR)
	{
		RecreateDevice();
		return;
	}

	// check if this is the first time we get the lost device
	if (deviceLost == false)
	{
		ReleaseDevice();
	}
	
	// se if we can recreate the device
	if (deviceState == D3DERR_DEVICENOTRESET)
	{
		RestoreDevice();
	}

	// device is lost but can not yet be recreated.
	if (deviceState == D3DERR_DEVICELOST)
	{
		// do nothing
	}
}


// Vertex Declaration Pool
void mini3d::D3D9GraphicsService::PoolVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration, count);

	// if the declaration is already pooled, just increase the counter
	if (vertexDeclarationPool.find(key) != vertexDeclarationPool.end())
	{
		vertexDeclarationPool[key].counter++;
		return;
	}

	// create a direct3d 9 vertex delcaration from the IVertexShader vertex declaration
	IDirect3DVertexDeclaration9* pVertexDeclaration = CreateD3D9VertexDeclaration(vertexDeclaration, count);

	// pool the vertex declaration
	vertexDeclarationPool[key] = VertexDeclarationContainer(vertexDeclaration, count, pVertexDeclaration);
}

void mini3d::D3D9GraphicsService::ReleaseVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count)
{
	std::string key = CreateVertexDeclarationKey(vertexDeclaration, count);
	
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
std::string mini3d::D3D9GraphicsService::CreateVertexDeclarationKey(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count)
{
	std::string key = "";
	
	for (unsigned int i = 0; i < count; i++)
	{
		key += char(vertexDeclaration[i]);
	}
	return key;
}
IDirect3DVertexDeclaration9* mini3d::D3D9GraphicsService::CreateD3D9VertexDeclaration(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count)
{
	// It is not already pooled, we need to create a new one
	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[count + 1];
	
	// cumulative offset for the vertexelements
	int offset = 0;
	int textureUsageIndex = 0;
	int uvStream = 0;
	int positionUsageIndex = 0;
	int colorUsageIndex = 0;
	int normalUsageIndex = 0;

	for (unsigned int i = 0; i < count; i++)
	{
		// these are the same for all
		pVertexElements[i].Offset = offset;
		pVertexElements[i].Stream = 0;
		
		// set the specific parameters
		switch (vertexDeclaration[i])
		{
				
		case IVertexShader::POSITION_FLOAT3:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT3;
			pVertexElements[i].Usage = D3DDECLUSAGE_POSITION;
			pVertexElements[i].UsageIndex = positionUsageIndex++;
			offset += 12;
			break;
		case IVertexShader::POSITION_FLOAT4:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT4;
			pVertexElements[i].Usage = D3DDECLUSAGE_POSITION;
			pVertexElements[i].UsageIndex = positionUsageIndex++;
			offset += 16;
			break;
		case IVertexShader::COLOR_INT:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_D3DCOLOR;
			pVertexElements[i].Usage = D3DDECLUSAGE_COLOR;
			pVertexElements[i].UsageIndex = colorUsageIndex++;
			offset += 4;
			break;
		case IVertexShader::COLOR_FLOAT4:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT4;
			pVertexElements[i].Usage = D3DDECLUSAGE_COLOR;
			pVertexElements[i].UsageIndex = colorUsageIndex++;
			offset += 16;
			break;
		case IVertexShader::TEXTURECOORDINATE_FLOAT2:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT2;
			pVertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pVertexElements[i].UsageIndex = textureUsageIndex++;
			offset += 8;
			break;
		case IVertexShader::NORMAL_FLOAT3:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT3;
			pVertexElements[i].Usage = D3DDECLUSAGE_NORMAL;
			pVertexElements[i].UsageIndex = normalUsageIndex++;
			offset += 12;
			break;
		case IVertexShader::NORMAL_FLOAT4:
			pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
			pVertexElements[i].Type = D3DDECLTYPE_FLOAT4;
			pVertexElements[i].Usage = D3DDECLUSAGE_NORMAL;
			pVertexElements[i].UsageIndex = normalUsageIndex++;
			offset += 16;
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
void mini3d::D3D9GraphicsService::BeginScene(void)
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

	SetRenderStates();
	pDevice->BeginScene();
	isDrawingScene = true;
}
void mini3d::D3D9GraphicsService::EndScene(void)
{
	pDevice->EndScene();
	isDrawingScene = false;
}

// setting render states
void mini3d::D3D9GraphicsService::SetRenderStates()
{
	// Set Render States
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //D3DCULL_CCW
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));

	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//pDevice->SetRenderState(D3DRS_CLIPPING, true);
}



// Graphics Pipeline States ---------------------------------------------------

unsigned int mini3d::D3D9GraphicsService::GetMaxTextures() const
{
	return (unsigned int)deviceCaps.MaxSimultaneousTextures;
}
unsigned int mini3d::D3D9GraphicsService::GetMaxTextureSize() const
{
	return (unsigned int)min(deviceCaps.MaxTextureHeight, deviceCaps.MaxTextureWidth);
}
unsigned int mini3d::D3D9GraphicsService::GetPixelShaderVersion() const
{
	return (unsigned int)deviceCaps.PixelShaderVersion;
}
unsigned int mini3d::D3D9GraphicsService::GetVertexShaderVersion() const
{
	return (unsigned int)deviceCaps.VertexShaderVersion;
}



// Graphics Pipeline States ---------------------------------------------------

// Shader Program
mini3d::IShaderProgram* mini3d::D3D9GraphicsService::GetShaderProgram() const 
{
	return pCurrentShaderProgram;
}
void mini3d::D3D9GraphicsService::SetShaderProgram(IShaderProgram* pShaderProgram)
{
	SetVertexShader(((D3D9ShaderProgram*)pShaderProgram)->GetVertexShader());
	SetPixelShader(((D3D9ShaderProgram*)pShaderProgram)->GetPixelShader());

	pCurrentShaderProgram = pShaderProgram;
}

// Pixlel Shader
mini3d::IPixelShader* mini3d::D3D9GraphicsService::GetPixelShader(void) const
{
	return pCurrentPixelShader;
}
void mini3d::D3D9GraphicsService::SetPixelShader(IPixelShader* pPixelShader)
{
	if (pCurrentPixelShader == pPixelShader)
		return;

	if (pPixelShader == 0)
	{
		pDevice->SetPixelShader(0);
	}
	else
	{
		D3D9PixelShader* pD3D9PixelShader = (D3D9PixelShader*)pPixelShader;
		pDevice->SetPixelShader((IDirect3DPixelShader9*)pD3D9PixelShader->GetPixelShaderBuffer());
	}

	pCurrentPixelShader = pPixelShader;
}

// Vertex Shader
mini3d::IVertexShader* mini3d::D3D9GraphicsService::GetVertexShader(void) const
{
	return pCurrentVertexShader;
}
void mini3d::D3D9GraphicsService::SetVertexShader(IVertexShader* pVertexShader)
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
		D3D9VertexShader* pD3D9VertexShader = (D3D9VertexShader*)pVertexShader;
		pDevice->SetVertexShader((IDirect3DVertexShader9*)pD3D9VertexShader->GetVertexShaderBuffer());

		unsigned int count;
		std::string key = CreateVertexDeclarationKey(pD3D9VertexShader->GetVertexDeclaration(count), count);
		VertexDeclarationContainer v = vertexDeclarationPool[key];
		pDevice->SetVertexDeclaration(vertexDeclarationPool[key].direct3dDeclaration);
	}

	pCurrentVertexShader = pVertexShader;

}

// Texture
mini3d::ITexture* mini3d::D3D9GraphicsService::GetTexture(const unsigned int& index) const 
{
	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		throw Error::MINI3D_ERROR_TEXTURE_INDEX_OUTSIDE_VALID_RANGE;
	}

	return currentITextures[index];
}
void mini3d::D3D9GraphicsService::SetTexture(ITexture* pTexture, const unsigned int& index)
{	

	if (index > deviceCaps.MaxSimultaneousTextures)
	{
		throw Error::MINI3D_ERROR_TEXTURE_INDEX_OUTSIDE_VALID_RANGE;
	}

	// if texture already assigned, then there is no need to re-assign it
	if (pTexture == currentITextures[index])
	{
		return;
	}

	if (pTexture == 0)
	{
		pDevice->SetTexture(index, 0);
	}
	else
	{

		// this cast is "unfailable" (not exception caught). Whoever inherits from ITexture must also inherit from ID3D9Texture
		ID3D9Texture* pD3D9Texture = dynamic_cast<ID3D9Texture*>(pTexture);

		// we always set the wrapstyle too... if we are changing the texture the overhead of also changing wrap style is small.
		D3DTEXTUREADDRESS adressMode;

		switch(pD3D9Texture->GetWrapStyle())
		{
		case ITexture::WRAP_TILE:
			adressMode = D3DTADDRESS_WRAP;
			break;
		case ITexture::WRAP_MIRROR:
			adressMode = D3DTADDRESS_MIRROR;
			break;
		case ITexture::WRAP_CLAMP:
			adressMode = D3DTADDRESS_CLAMP;
			break;
		}

		// set the wrap style
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSU, adressMode);
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSV, adressMode);
		pDevice->SetSamplerState(index, D3DSAMP_ADDRESSW, adressMode);

		pDevice->SetSamplerState(index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice->SetSamplerState(index, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pDevice->SetSamplerState(index, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


		// set the texture
		pDevice->SetTexture(index, pD3D9Texture->GetTextureBuffer());
	}
	currentITextures[index] = pTexture;
}

// Render Target
mini3d::IRenderTarget* mini3d::D3D9GraphicsService::GetRenderTarget(void) const
{
	return pCurrentRenderTarget;
}
void mini3d::D3D9GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{
	// This method is tricky and complex because if the user sets a fullscreen render target
	// we need to reset the device to get a new default fullscreen swapchain (and the opposite when we switch back...)
	

	// This is a dynamic cast used as a typecheck, code police says this should be solved with virtual function calls instead
	D3D9WindowRenderTarget* pD3D9WindowRenderTarget = dynamic_cast<D3D9WindowRenderTarget*>(pRenderTarget);


	// ---------- SETTING DEFAULT RENDER TARGET -------------------------------
	if (pRenderTarget == 0)
	{
		// Dont set the rendertarget if it is already set
		if (pRenderTarget == pCurrentRenderTarget)
			return;

		pDevice->SetRenderTarget(0, pDefaultRenderTarget);
		pDevice->SetDepthStencilSurface(pDefaultDepthStencilSurface);
		
		pCurrentRenderTarget = pRenderTarget;
		return;	
	}

	// pDefaultSwapChain == 0 if device was not created in fullscreen
	// Setting the render target to 0 or the pDefaultSwapChain will restore the default render target
	if ((pRenderTarget == pDefaultSwapChain) && (pD3D9WindowRenderTarget != 0) && (pD3D9WindowRenderTarget->GetScreenState() == IWindowRenderTarget::SCREEN_STATE_FULLSCREEN))
	{
		// Dont set the rendertarget if it is already set
		if (pRenderTarget == pCurrentRenderTarget)
			return;

		pDevice->SetRenderTarget(0, pDefaultRenderTarget);
		pDevice->SetDepthStencilSurface(pDefaultDepthStencilSurface);
		
		pCurrentRenderTarget = pRenderTarget;
		return;
	}

	// ---------- SCREEN RENDER TARGETS ------------------------------------------

	// If the type check above succeded we need to check if we should change the default swap chain
	if (pD3D9WindowRenderTarget != 0)
	{

		bool isFullscreen = (pD3D9WindowRenderTarget->GetScreenState() == IWindowRenderTarget::SCREEN_STATE_FULLSCREEN);

		// if this rendertarget is already the current and the correct screen state is set, return
		if ((pRenderTarget == pCurrentRenderTarget) && (isFullscreen == GetIsFullScreen()))
			return;

		// if the device is not created for the screen mode we want to set, we need to recreate the device
		if (isFullscreen != GetIsFullScreen())
		{
			// Set the current rendertarget to the new screen render target so it gets set when we restore the device
			pCurrentRenderTarget = pD3D9WindowRenderTarget;

			// Set the pDefualtSwapChain depending on screen state
			pDefaultSwapChain = isFullscreen ? pD3D9WindowRenderTarget : 0;

			// Reset the device with the new fullscreen or windowed setup
			ResetDevice();

			// done
			return;
		}
	}

	// ---------- ALL OTHER CASES ---------------------------------------------

	// Dont set the rendertarget if it is already set
	if (pRenderTarget == pCurrentRenderTarget)
		return;

	// this cast is "unfailable" (will never return NULL), whoever inherits from IRenderTarget must also inherit from ID3D9RenderTarget
	ID3D9RenderTarget* pD3D9RenderTarget = dynamic_cast<ID3D9RenderTarget*>(pRenderTarget);

	// Get the render target buffer from the render target
	IDirect3DSurface9* pRenderTargetBuffer = (IDirect3DSurface9*)pD3D9RenderTarget->GetRenderTargetBuffer();
	
	// set the render target
	pDevice->SetRenderTarget(0, pRenderTargetBuffer);
	pCurrentRenderTarget = pRenderTarget;

	// Set the depth stencil if we have one
	if (pRenderTarget->GetDepthTestEnabled() == true)
	{
		pDevice->SetDepthStencilSurface(pD3D9RenderTarget->GetDepthStencil());

		pDevice->SetRenderState(D3DRS_ZENABLE, true);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}
	else
	{
		pDevice->SetDepthStencilSurface(0);

		pDevice->SetRenderState(D3DRS_ZENABLE, false);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}
}

// Index Buffer
mini3d::IIndexBuffer* mini3d::D3D9GraphicsService::GetIndexBuffer(void) const
{	
	return pCurrentIndexBuffer;
}
void mini3d::D3D9GraphicsService::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
{
	if (pCurrentIndexBuffer == pIndexBuffer)
		return;

	if (pIndexBuffer == 0)
	{
		pDevice->SetIndices(0);
	}
	else
	{
		D3D9IndexBuffer* pD3D9IndexBuffer = (D3D9IndexBuffer*)pIndexBuffer;
		pDevice->SetIndices(pD3D9IndexBuffer->GetIndexBuffer());
		
		// set the cullMode
		if (pIndexBuffer->GetCullMode() != currentCullMode)
		{
			if (pIndexBuffer->GetCullMode() == IIndexBuffer::CULL_NONE)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else if (pIndexBuffer->GetCullMode() == IIndexBuffer::CULL_CLOCKWIZE)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			}
			else
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
		}
	}

	pCurrentIndexBuffer = pIndexBuffer;
}

// Vertex Buffer
mini3d::IVertexBuffer* mini3d::D3D9GraphicsService::GetVertexBuffer(void) const
{
	return pCurrentVertexBuffer;
}
void mini3d::D3D9GraphicsService::SetVertexBuffer(IVertexBuffer* pVertexBuffer)
{
	if (pCurrentVertexBuffer == pVertexBuffer)
		return;

	if (pVertexBuffer == 0)
	{
		pDevice->SetStreamSource(0, 0, 0, 0);
	}
	else
	{
		D3D9VertexBuffer* pD3D9VertexBuffer = (D3D9VertexBuffer*)pVertexBuffer;
		pDevice->SetStreamSource(0, pD3D9VertexBuffer->GetVertexBuffer(), 0, pD3D9VertexBuffer->GetVertexSizeInBytes());
	}

	pCurrentVertexBuffer = pVertexBuffer;
}

// Shader Parameters
void mini3d::D3D9GraphicsService::SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count)
{
	pDevice->SetVertexShaderConstantF(index, pData, count / 4);
}
void mini3d::D3D9GraphicsService::SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count)
{
	pDevice->SetVertexShaderConstantI(index, pData, count);
}
void mini3d::D3D9GraphicsService::SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns)
{
	if (rows == 4 && columns == 4)
	{
		// transpose the matrix
		float* matrixTransposed = new float[16];
		for (int i = 0; i < 16; i++)
		{
			matrixTransposed[i] = pData[(i % 4) * 4 + (i / 4)];
		}
		pDevice->SetVertexShaderConstantF(index, matrixTransposed, 4);
		delete[] matrixTransposed;
	}
}


// Drawing
void mini3d::D3D9GraphicsService::Draw(void)
{
	if (pCurrentVertexBuffer == 0 || pCurrentIndexBuffer == 0)
		return;

	BeginScene();
	
	if (deviceLost == false)
	{
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), 0, pCurrentIndexBuffer->GetIndexCount() / 3);
	}
}
void mini3d::D3D9GraphicsService::DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices)
{
	BeginScene();
	D3D9VertexBuffer* pD3D9VertexBuffer = (D3D9VertexBuffer*)pCurrentVertexBuffer;
	
	if (deviceLost == false)
	{
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pCurrentVertexBuffer->GetVertexCount(), startIndex, numIndices);
	}
}

// Clear
void mini3d::D3D9GraphicsService::Clear(const float& r, const float& g, const float& b, const float& a)
{

	DWORD flags = D3DCLEAR_TARGET;
	
	// if we have a depthstencil we need to clear that too
	if (pCurrentRenderTarget->GetDepthTestEnabled() == true)
	{
		flags |= D3DCLEAR_ZBUFFER;
	}

	// Pack the color into bits.
	unsigned int red = static_cast<unsigned int>(r * 255) << 16;
	unsigned int green = static_cast<unsigned int>(g * 255) << 8;
	unsigned int blue = static_cast<unsigned int>(b * 255);
	unsigned int alpha = static_cast<unsigned int>(a * 255) << 24;
	DWORD color = alpha | red | green | blue;

	pDevice->Clear(0, 0, flags, color, 1.0f, 0);
}


// Create Resources
mini3d::IWindowRenderTarget* mini3d::D3D9GraphicsService::CreateWindowRenderTarget(const int& hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality)
{
	return new D3D9WindowRenderTarget(this, hWindow, depthTestEnabled, quality);
}
mini3d::IRenderTargetTexture* mini3d::D3D9GraphicsService::CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{
	return new D3D9RenderTargetTexture(this, width, height, depthTestEnabled);
}
mini3d::IBitmapTexture* mini3d::D3D9GraphicsService::CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle)
{
	return new D3D9BitmapTexture(this, pBitmap, width, height, bitDepth, wrapStyle);
}
mini3d::IVertexBuffer* mini3d::D3D9GraphicsService::CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes)
{
	return new D3D9VertexBuffer(this, pVertices, count, vertexSizeInBytes);
}
mini3d::IIndexBuffer* mini3d::D3D9GraphicsService::CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType, const IIndexBuffer::CullMode& cullMode)
{
	return new D3D9IndexBuffer(this, pIndices, count, dataType, cullMode);
}
mini3d::IPixelShader* mini3d::D3D9GraphicsService::CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes)
{
	return new D3D9PixelShader(this, shaderBytes, sizeInBytes);
}
mini3d::IVertexShader* mini3d::D3D9GraphicsService::CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount)
{
	return new D3D9VertexShader(this, shaderBytes, sizeInBytes, vertexDeclaration, vertexDataCount);
}
mini3d::IShaderProgram* mini3d::D3D9GraphicsService::CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader)
{
	return new D3D9ShaderProgram(this, pVertexShader, pPixelShader);
}
