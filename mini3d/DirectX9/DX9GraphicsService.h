
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9GRAPHICSSERVICE_H
#define MINI3D_DX9GRAPHICSSERVICE_H

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
#include "DX9WindowRenderTarget.h"


namespace mini3d
{

struct VertexDeclarationContainer
{

	VertexDeclarationContainer(IVertexShader::VertexDataType mini3dDeclaration[], const unsigned int& count, IDirect3DVertexDeclaration9* direct3dDeclaration) : 
								mini3dDeclaration(mini3dDeclaration), direct3dDeclaration(direct3dDeclaration), count(count), counter(1)
	{

	}

	VertexDeclarationContainer() : 
	mini3dDeclaration(0), count(count), direct3dDeclaration(0), counter(0)
	{

	}

	VertexDeclarationContainer( VertexDeclarationContainer& other)
	{
		this->count = other.count;
		this->counter = other.counter;
		this->direct3dDeclaration = other.direct3dDeclaration;
		this->mini3dDeclaration = other.mini3dDeclaration;
	}

	IVertexShader::VertexDataType* mini3dDeclaration;
	IDirect3DVertexDeclaration9* direct3dDeclaration;
	int count;
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
friend class DX9WindowRenderTarget;
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
	IShaderProgram* pLostDeviceShaderProgram;
	ITexture** lostDeviceCurrentITextures;

	// other state tracking
	bool isDrawingScene;
	
	IIndexBuffer::CullMode currentCullMode;

	// other variables
	HWND hWindow;
	HWND hInternalWindow;
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	bool deviceLost;
	bool isFullscreen;
	bool defaultDepthStencil;

	//IDirect3DSurface9* pCurrentDepthStencilBuffer;
	IDirect3DSurface9* pDefaultRenderTarget;
	IDirect3DSurface9* pDefaultDepthStencilSurface;
	IWindowRenderTarget* pDefaultSwapChain;
	D3DPRESENT_PARAMETERS presentationParameters;

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	DX9GraphicsService();
	~DX9GraphicsService(void);


	// IGRAPHICS SERVICE INTERFACE --------------------------------------------

	// Properties
	virtual bool GetIsFullScreen()
	{ 
		return pDefaultSwapChain != 0; 
	};


	// Get Graphics Card Capabilities
	virtual unsigned int GetMaxTextures() const;
	virtual unsigned int GetMaxTextureSize() const;
	virtual unsigned int GetPixelShaderVersion() const;
	virtual unsigned int GetVertexShaderVersion() const;

	// Create Resources
	virtual IWindowRenderTarget* CreateWindowRenderTarget(const int& hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality);
//	virtual IFullscreenRenderTarget* CreateFullscreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& hWindow, const bool& depthTestEnabled, const IFullscreenRenderTarget::Quality& quality);
	virtual IRenderTargetTexture* CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	virtual IBitmapTexture* CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	virtual IVertexBuffer* CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes);
	virtual IIndexBuffer* CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType = IIndexBuffer::INT_16, const IIndexBuffer::CullMode& cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE);
	virtual IPixelShader* CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes);
	virtual IVertexShader* CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader);

	// Pipeline States
	virtual IPixelShader* GetPixelShader() const;
	virtual void SetPixelShader(IPixelShader* pPixelShader);
	
	virtual IVertexShader* GetVertexShader() const;
	virtual void SetVertexShader(IVertexShader* pVertexShader);

	virtual IShaderProgram* GetShaderProgram() const;
	virtual void SetShaderProgram(IShaderProgram* ShaderProgram);
	
	virtual ITexture* GetTexture(const unsigned int& index) const;
	virtual void SetTexture(ITexture* pTexture, const unsigned int& index);
	
	virtual IRenderTarget* GetRenderTarget() const;
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget);
	
	virtual IDepthStencil* GetDepthStencil() const;
	virtual void SetDepthStencil(IDepthStencil* pDepthStencil);

	// Shader Parameters
	virtual void SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count);
	virtual void SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count);
	virtual void SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns);
	
	virtual IIndexBuffer* GetIndexBuffer() const;
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer);
	
	virtual IVertexBuffer* GetVertexBuffer() const;
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer);

	// Drawing
	virtual void Clear(const float& r, const float& g, const float& b, const float& a);
	virtual void Draw();
	virtual void DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices);

	
	// PUBLIC METHODS ------------------------------------------------------------
	
	// These functions are accessed by the DX9Resources
	IDirect3DDevice9* GetDevice();
	IDirect3D9* GetDirect3D() { return pD3D; };
	D3DPRESENT_PARAMETERS GetPresentationParameters();

private:

	// INTERNAL HELPER FUNCTIONS ----------------------------------------------

	// Device creation
	void CreateDevice();
	void CreateInternalWindow();

	// Drawing Graphics
	virtual void BeginScene();
	virtual void EndScene();
	virtual void SetRenderStates();

	// Resource Management
	void UpdateResources();
	void UnloadResources();
	void AddResource(IDX9Resource* resource);
	void RemoveResource(IDX9Resource* resource);
	
	// Lost device stuff
	void SaveGraphicsState();
	void RestoreGraphicsState();
	void HandleLostDevice();
	void RecreateDevice();
	void ResetDevice();
	void ReleaseDevice();
	void RestoreDevice();

	// Vertex declaration pooling
	void PoolVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	void ReleaseVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	IDirect3DVertexDeclaration9* mini3d::DX9GraphicsService::CreateDX9VertexDeclaration(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	std::string CreateVertexDeclarationKey(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);

};
}

#endif
