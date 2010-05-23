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

#ifndef AURORA_DX9GRAPHICSSERVICE_H
#define AURORA_DX9GRAPHICSSERVICE_H

#include <d3d9.h>
#include <set>
#include <map>
#include <string>
#include "../IGraphicsService.h"

#include "internal/IDX9RenderTarget.h"
#include "internal/IDX9Texture.h"
#include "internal/IDX9Resource.h"

#include "DX9BitmapTexture.h"
#include "DX9IndexBuffer.h"
#include "DX9PixelShader.h"
#include "DX9VertexShader.h"
#include "DX9ShaderProgram.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9RenderTargetTexture.h"
#include "DX9DepthStencil.h"
#include "DX9ScreenRenderTarget.h"
#include "DX9FullscreenRenderTarget.h"


namespace mini3d
{

struct VertexDeclarationContainer
{
	VertexDeclarationContainer(IVertexShader::VertexDeclarationVector mini3dDeclaration, IDirect3DVertexDeclaration9* direct3dDeclaration, int counter) : 
								mini3dDeclaration(mini3dDeclaration), direct3dDeclaration(direct3dDeclaration), counter(counter)
	{
	}

	VertexDeclarationContainer() : 
	mini3dDeclaration(IVertexShader::VertexDeclarationVector()), direct3dDeclaration(0), counter(0)
	{
	}

	VertexDeclarationContainer( VertexDeclarationContainer& other)
	{
		this->counter = other.counter;
		this->direct3dDeclaration = other.direct3dDeclaration;
		this->mini3dDeclaration = other.mini3dDeclaration;
	}

	IVertexShader::VertexDeclarationVector mini3dDeclaration;
	IDirect3DVertexDeclaration9* direct3dDeclaration;
	int counter;
};
	
class DX9GraphicsService : public IGraphicsService
{
	// friends
friend class DX9BitmapTexture;
friend class DX9RenderTargetTexture;
friend class DX9PixelShader;
friend class DX9VertexShader;
friend class DX9ShaderProgram;
friend class DX9VertexBuffer;
friend class DX9IndexBuffer;
friend class DX9DepthStencil;
friend class DX9ScreenRenderTarget;
friend class DX9FullscreenRenderTarget;

private:
	
	// These member variables are accessed by the friend classes
	typedef std::set<IDX9Resource*> ResourceContainer;
	ResourceContainer resourceList;

	typedef std::pair<unsigned int, IDirect3DVertexDeclaration9*> counterPair;
	typedef std::map<std::string, VertexDeclarationContainer> VertexDeclarationPool;
	VertexDeclarationPool vertexDeclarationPool;

	// Member variables
	D3DCAPS9 deviceCaps;
	
	// currently loaded resources
	IRenderTarget* pCurrentRenderTarget;
	IDirect3DSurface9* pCurrentRenderTargetBuffer;
	IDepthStencil* pCurrentDepthStencil;
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;
	IPixelShader* pCurrentPixelShader;
	IVertexShader* pCurrentVertexShader;
	ITexture** currentITextures;
	IShaderProgram* pCurrentShaderProgram;

	// Device lost graphics state
	IRenderTarget* pLostDeviceRenderTarget;
	IDepthStencil* pLostDeviceDepthStencil;
	IVertexBuffer* pLostDeviceVertexBuffer;
	IIndexBuffer* pLostDeviceIndexBuffer;
	IPixelShader* pLostDevicePixelShader;
	IVertexShader* pLostDeviceVertexShader;
	ITexture** lostDeviceCurrentITextures;

	// other state tracking
	bool isDrawingScene;
	
	// other variables
	HWND hWindow;
	HWND hInternalWindow;
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	bool deviceLost;
	bool isFullscreen;
	IDX9RenderTarget* pFullscreenRenderTarget;
	bool defaultDepthStencil;

	//IDirect3DSurface9* pCurrentDepthStencilBuffer;
	IDirect3DSurface9* pDefaultRenderTarget;
	IDirect3DSurface9* pDefaultDepthStencilSurface;
	D3DPRESENT_PARAMETERS presentationParameters;

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	DX9GraphicsService(bool isFullscreen);
	~DX9GraphicsService(void);


	// IGRAPHICS SERVICE INTERFACE --------------------------------------------

	// Properties
	virtual bool GetIsFullScreen()
	{ 
		return isFullscreen; 
	};
	virtual void SetIsFullScreen(bool value)
	{
		if (isFullscreen == value)
			return;

		isFullscreen = value;
		
		// Recreate The device
		RecreateDevice();
	};

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures(void);
	virtual int GetMaxTextureSize(void);
	virtual int GetPixelShaderVersion(void);
	virtual int GetVertexShaderVersion(void);

	// Create Resources
	virtual IScreenRenderTarget* CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IScreenRenderTarget::Quality quality);
	virtual IFullscreenRenderTarget* CreateFullscreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IFullscreenRenderTarget::Quality quality);
	virtual IRenderTargetTexture* CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled);
	virtual IBitmapTexture* CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE);
	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration);
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices,  unsigned int count, const IIndexBuffer::CullMode cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE);
	virtual IPixelShader* CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes);
	virtual IVertexShader* CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	virtual IShaderProgram* CreateShaderProgram(mini3d::IVertexShader* pVertexShader, mini3d::IPixelShader* pPixelShader);

	// Pipeline States
	virtual IPixelShader* GetPixelShader(void);
	virtual void SetPixelShader(IPixelShader* pPixelShader);
	
	virtual IVertexShader* GetVertexShader(void);
	virtual void SetVertexShader(IVertexShader* pVertexShader);

	virtual IShaderProgram* GetShaderProgram(void);
	virtual void SetShaderProgram(IShaderProgram* ShaderProgram);
	
	virtual ITexture* GetTexture(unsigned int index);
	virtual void SetTexture(ITexture* pTexture, unsigned int index);
	
	virtual IRenderTarget* GetRenderTarget(void);
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget);
	
	virtual IDepthStencil* GetDepthStencil(void);
	virtual void SetDepthStencil(IDepthStencil* pDepthStencil);

	// Shader Parameters
	virtual void SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count);
	virtual void SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count);
	virtual void SetShaderParameterMatrix(unsigned int index, const float* pData, unsigned int rows, unsigned int columns);
	
	virtual IIndexBuffer* GetIndexBuffer(void);
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer);
	
	virtual IVertexBuffer* GetVertexBuffer(void);
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer);

	// Drawing
	virtual void Clear(int color);
	virtual void Draw(void);
	virtual void DrawIndices(unsigned int startIndex, unsigned int numIndices);

private:
	
	// INTERNAL HELPER FUNCTIONS ----------------------------------------------
	
	// These functions are accessed by the DX9Resources
	IDirect3DDevice9* GetDevice(void);
	D3DPRESENT_PARAMETERS GetPresentationParameters(void);
	void CheckMultisampleFormat(IScreenRenderTarget::Quality& quality, bool fullscreen);
	D3DMULTISAMPLE_TYPE FromMultisampleFormat(IScreenRenderTarget::Quality quality);

	// INTERNAL HELPER FUNCTIONS ----------------------------------------------

	// Device creation
	void CreateDevice();
	void CreateInternalWindow(void);
	D3DFORMAT GetCorrectBackBufferFormat(void);
	D3DFORMAT GetCorrectDepthStencilFormat(void);

	// Drawing Graphics
	virtual void BeginScene(void);
	virtual void EndScene(void);
	virtual void SetRenderStates(void);

	// Resource Management
	void UpdateResources(void);
	void UnloadResources(void);
	void AddResource(IDX9Resource* resource);
	void RemoveResource(IDX9Resource* resource);
	
	// Lost device stuff
	void SaveGraphicsState(void);
	void RestoreGraphicsState(void);
	void HandleLostDevice(void);
	void RecreateDevice(void);
	void ReleaseDevice(void);
	void RestoreDevice(void);

	// Vertex declaration pooling
	void PoolVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	void ReleaseVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	IDirect3DVertexDeclaration9* mini3d::DX9GraphicsService::CreateDX9VertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	std::string CreateVertexDeclarationKey(const IVertexShader::VertexDeclarationVector& vertexDeclaration);

};
}

#endif
