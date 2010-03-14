
#ifndef AURORA_IGRAPHICSSERVICE_H
#define AURORA_IGRAPHICSSERVICE_H


#include "GraphicsSettings.h"
#include "IPixelShader.h"
#include "IVertexShader.h"
#include "ITexture.h"
#include "IRenderTarget.h"
#include "IDepthStencil.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "IRenderTargetBuffer.h"

class IGraphicsService
{
public:

	// initialisation
	virtual ~IGraphicsService(void) { };
	
	virtual void Release(void) = 0;
	
	virtual void SetSettings(const GraphicsSettings& graphicsSettings) = 0;
	virtual GraphicsSettings GetSettings(void) = 0;

	// States
	virtual IPixelShader* GetPixelShader(void) = 0;
	virtual void SetPixelShader(IPixelShader* pPixelShader) = 0;

	virtual IVertexShader* GetVertexShader(void) = 0;
	virtual void SetVertexShader(IVertexShader* pVertexShader) = 0;

	virtual ITexture* GetTexture(unsigned int index) = 0;
	virtual void SetTexture(ITexture* pTexture, unsigned int index) = 0;

	virtual IRenderTarget* GetRenderTarget(void) = 0;
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget) = 0;

	virtual IDepthStencil* GetDepthStencil(void) = 0;
	virtual void SetDepthStencil(IDepthStencil* pDepthStencil) = 0;

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures() = 0;
	virtual int GetMaxTextureSize() = 0;
	virtual int GetPixelShaderVersion() = 0;
	virtual int GetVertexShaderVersion() = 0;

	// Drawing Graphics
	virtual void BeginFrame(void) = 0; // before frame is started, resources will be updated
	virtual void EndFrame(void) = 0;
	virtual void BeginDraw(void) = 0;
	virtual void EndDraw(void) = 0;

	virtual void SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count) = 0;
	virtual void SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count) = 0;
	virtual void SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count) = 0;

	virtual IIndexBuffer* GetIndexBuffer(void) = 0;
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer) = 0;

	virtual IVertexBuffer* GetVertexBuffer(void) = 0;
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

	virtual void Draw(void) = 0;
	virtual void DrawIndices(unsigned int startIndex, unsigned int numIndices) = 0;

	virtual void ClearRenderTarget(int color) = 0;
	virtual void ClearDepthStencil(void) = 0;

	// Create Resources
	virtual IRenderTargetBuffer* CreateRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow) = 0;

	virtual IRenderTarget* CreateRenderTarget(unsigned int width, unsigned int height) = 0;
	virtual IDepthStencil* CreateDepthStencil(unsigned int width, unsigned int height) = 0;

	virtual ITexture* CreateTexture(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle = ITexture::TILE, ITexture::BitDepth bitDepth = ITexture::BIT32) = 0;

	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const VertexDeclaration& vertexDeclaration) = 0;
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices, unsigned int count) = 0;

	virtual IPixelShader* CreatePixelShader(char* pShaderBytes, unsigned int sizeInBytes) = 0;
	virtual IVertexShader* CreateVertexShader(char* pShaderBytes, unsigned int sizeInBytes) = 0;

};

#endif
