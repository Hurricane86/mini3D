
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_D3D9GRAPHICSSERVICE_H
#define MINI3D_D3D9GRAPHICSSERVICE_H

#include <d3d9.h>
#include <set>
#include <map>
#include <string>
#include "../mini3d/IGraphicsService.h"

#include "internal/ID3D9RenderTarget.h"
#include "internal/ID3D9Texture.h"
#include "internal/ID3D9Resource.h"

#include "D3D9BitmapTexture.h"
#include "D3D9IndexBuffer.h"
#include "D3D9PixelShader.h"
#include "D3D9VertexShader.h"
#include "D3D9ShaderProgram.h"
#include "D3D9VertexBuffer.h"
#include "D3D9IndexBuffer.h"
#include "D3D9RenderTargetTexture.h"
#include "D3D9WindowRenderTarget.h"


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
	
class D3D9GraphicsService : public IGraphicsService
{
	// friends
friend class D3D9BitmapTexture;
friend class D3D9RenderTargetTexture;
friend class D3D9PixelShader;
friend class D3D9VertexShader;
friend class D3D9ShaderProgram;
friend class D3D9VertexBuffer;
friend class D3D9IndexBuffer;
friend class D3D9DepthStencil;
friend class D3D9WindowRenderTarget;
friend class D3D9FullscreenRenderTarget;

private:
	
	// These member variables are accessed by the friend classes
	typedef std::set<ID3D9Resource*> ResourceContainer;
	ResourceContainer resourceList;

	typedef std::pair<unsigned int, IDirect3DVertexDeclaration9*> counterPair;
	typedef std::map<std::string, VertexDeclarationContainer> VertexDeclarationPool;
	VertexDeclarationPool vertexDeclarationPool;

	// Member variables
	D3DCAPS9 deviceCaps;
	
	// currently loaded resources
	IRenderTarget* pCurrentRenderTarget;
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;
	IPixelShader* pCurrentPixelShader;
	IVertexShader* pCurrentVertexShader;
	ITexture** currentITextures;
	IShaderProgram* pCurrentShaderProgram;

	// Device lost graphics state
	IRenderTarget* pLostDeviceRenderTarget;
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
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	bool deviceLost;
	bool isFullscreen;
	bool defaultDepthStencil;

	IDirect3DSurface9* pDefaultRenderTarget;
	IDirect3DSurface9* pDefaultDepthStencilSurface;
	IWindowRenderTarget* pDefaultSwapChain;
	D3DPRESENT_PARAMETERS presentationParameters;

public:
	static D3D9GraphicsService* graphicsService;

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	D3D9GraphicsService();
	~D3D9GraphicsService(void);


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
	virtual IWindowRenderTarget* CreateWindowRenderTarget(const MINI3D_WINDOW hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality);
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
	
	// These functions are accessed by the D3D9Resources
	IDirect3DDevice9* GetDevice();
	IDirect3D9* GetDirect3D() { return pD3D; };
	D3DPRESENT_PARAMETERS GetPresentationParameters();
	void RecreateDevice();

private:

	// INTERNAL HELPER FUNCTIONS ----------------------------------------------

	// Init and Dispose
	void Init();
	void Dispose();

	// Device creation
	void CreateInternalWindow();
	void DisposeInternalWindow();

	// Drawing Graphics
	virtual void BeginScene();
	virtual void EndScene();
	virtual void SetRenderStates();

	// Resource Management
	void UpdateResources();
	void UnloadResources();
	void AddResource(ID3D9Resource* resource);
	void RemoveResource(ID3D9Resource* resource);
	
	// Lost device stuff
	void SaveGraphicsState();
	void RestoreGraphicsState();
	void HandleLostDevice();

	void TearDownDevice();
	void CreateDevice();

	void ResetDevice();
	void ReleaseDevice();
	void RestoreDevice();

	// Vertex declaration pooling
	void PoolVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	void ReleaseVertexDeclaration(IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	IDirect3DVertexDeclaration9* mini3d::D3D9GraphicsService::CreateD3D9VertexDeclaration(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);
	std::string CreateVertexDeclarationKey(const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& count);

};
}

#endif
#endif
