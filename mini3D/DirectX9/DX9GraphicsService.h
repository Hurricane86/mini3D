#ifndef AURORA_DX9GRAPHICSSERVICE_H
#define AURORA_DX9GRAPHICSSERVICE_H

#include <d3d9.h>
#include <set>
#include "../IGraphicsService.h"
#include "IDX9Resource.h"
#include "DX9Texture.h"
#include "DX9IndexBuffer.h"
#include "DX9PixelShader.h"
#include "DX9VertexShader.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9RenderTarget.h"
#include "DX9DepthStencil.h"
#include "DX9RenderTargetBuffer.h"


class DX9GraphicsService : public IGraphicsService
{
friend class DX9Texture;
friend class DX9RenderTarget;
friend class DX9PixelShader;
friend class DX9VertexShader;
friend class DX9VertexBuffer;
friend class DX9IndexBuffer;
friend class DX9DepthStencil;
friend class DX9RenderTargetBuffer;

public:
	// Constructor
	DX9GraphicsService(const GraphicsSettings& graphicsSettings, int hWindow);
	~DX9GraphicsService(void);

private:
	// Member variables

	typedef std::set<IDX9Resource*> ResourceContainer;
	private: ResourceContainer resourceList;

	int hWindow;
	GraphicsSettings graphicsSettings;
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	IPixelShader* pCurrentPixelShader;
	IVertexShader* pCurrentVertexShader;
	ITexture* pCurrentTexture;
	IRenderTarget* pCurrentRenderTarget;
	IDepthStencil* pCurrentDepthStencil;
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;

	D3DPRESENT_PARAMETERS presentationParameters;

private:
	// Support Functions
	IDirect3DDevice9* GetDevice(void);
	D3DPRESENT_PARAMETERS GetPresentationParameters(void);

	virtual void CreateDevice(void);

	// Resource Management
	void UpdateResources(void);
	void AddResource(IDX9Resource* resource);
	void RemoveResource(IDX9Resource* resource);

public:
	// IGraphicsService
	virtual void Release(void);
	
	virtual void SetSettings(const GraphicsSettings& graphicsSettings);
	virtual GraphicsSettings GetSettings(void);
	
	// States	
	virtual IPixelShader* GetPixelShader(void);
	virtual void SetPixelShader(IPixelShader* pPixelShader);
	
	virtual IVertexShader* GetVertexShader(void);
	virtual void SetVertexShader(IVertexShader* pVertexShader);
	
	virtual ITexture* GetTexture(unsigned int index);
	virtual void SetTexture(ITexture* pTexture, unsigned int index);
	
	virtual IRenderTarget* GetRenderTarget(void);
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget);
	
	virtual IDepthStencil* GetDepthStencil(void);
	virtual void SetDepthStencil(IDepthStencil* pDepthStencil);
	
	// Get Graphics Card Capabilities
	virtual int GetMaxTextures(void);
	virtual int GetMaxTextureSize(void);
	virtual int GetPixelShaderVersion(void);
	virtual int GetVertexShaderVersion(void);
	
	// Drawing Graphics
	virtual void BeginFrame(void);
	virtual void EndFrame(void);
	virtual void BeginDraw(void);
	virtual void EndDraw(void);

	virtual void SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count);
	virtual void SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count);
	virtual void SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count);
	
	virtual IIndexBuffer* GetIndexBuffer(void);
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer);
	
	virtual IVertexBuffer* GetVertexBuffer(void);
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer);

	virtual void Draw(void);
	virtual void DrawIndices(unsigned int startIndex, unsigned int numIndices);

	virtual void ClearRenderTarget(int color);
	virtual void ClearDepthStencil(void);

	// Create Resources
	virtual IRenderTargetBuffer* CreateRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow);

	virtual IRenderTarget* CreateRenderTarget(unsigned int width, unsigned int height);
	virtual IDepthStencil* CreateDepthStencil(unsigned int width, unsigned int height);

	virtual ITexture* CreateTexture(void* pBitmap, unsigned int width, unsigned int height);

	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const VertexDeclaration& vertexDeclaration);
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices, unsigned int count);

	virtual IPixelShader* CreatePixelShader(char* pShaderBytes, unsigned int sizeInBytes);
	virtual IVertexShader* CreateVertexShader(char* pShaderBytes, unsigned int sizeInBytes);
};

#endif
