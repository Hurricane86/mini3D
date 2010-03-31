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
#include "../GraphicsSettings.h"
#include "../IGraphicsService.h"

#include "internal/IDX9RenderTarget.h"
#include "internal/IDX9Texture.h"
#include "internal/IDX9Resource.h"

#include "DX9BitmapTexture.h"
#include "DX9IndexBuffer.h"
#include "DX9PixelShader.h"
#include "DX9VertexShader.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9RenderTargetTexture.h"
#include "DX9DepthStencil.h"
#include "DX9ScreenRenderTarget.h"


class DX9GraphicsService : public IGraphicsService
{
friend class DX9BitmapTexture;
friend class DX9RenderTargetTexture;
friend class DX9PixelShader;
friend class DX9VertexShader;
friend class DX9VertexBuffer;
friend class DX9IndexBuffer;
friend class DX9DepthStencil;
friend class DX9ScreenRenderTarget;

public:
	// Constructor
	DX9GraphicsService(const GraphicsSettings& graphicsSettings, int hWindow);
	~DX9GraphicsService(void);

private:
	
	// These member variables are accessed by the friend classes
	typedef std::set<IDX9Resource*> ResourceContainer;
	private: ResourceContainer resourceList;

	typedef std::pair<unsigned int, IDirect3DVertexDeclaration9*> counterPair;
	typedef std::map<std::string, counterPair> VertexDeclarationPool;
	private: VertexDeclarationPool vertexDeclarationPool;

	// Member variables

	D3DCAPS9 deviceCaps;

	int hWindow;
	GraphicsSettings graphicsSettings;
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	IPixelShader* pCurrentPixelShader;
	IVertexShader* pCurrentVertexShader;
	
	IDirect3DTexture9** currentTextures;
	ITexture** currentITextures;
	
	ITexture::WrapStyle* currentWrapStyles;
	
	IRenderTarget* pCurrentRenderTarget;
	
	IDirect3DSurface9* pCurrentDepthStencilBuffer;
	
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;

	IDirect3DSurface9* pDefaultRenderTarget;

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
	void PoolVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	void ReleaseVertexDeclaration(const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	std::string CreateVertexDeclarationKey(const IVertexShader::VertexDeclarationVector& vertexDeclaration);

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
//
//public:

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
//	virtual void ClearDepthStencil(void);

	// Create Resources
	virtual IScreenRenderTarget* CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled);

	virtual IRenderTargetTexture* CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled);
	//virtual IDepthStencil* CreateDepthStencil(unsigned int width, unsigned int height);

	virtual IBitmapTexture* CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE);

	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration);
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices, unsigned int count);

	virtual IPixelShader* CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes);
	virtual IVertexShader* CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration);
};

#endif
