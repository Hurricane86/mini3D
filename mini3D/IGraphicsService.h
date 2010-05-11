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

#ifndef AURORA_IGRAPHICSSERVICE_H
#define AURORA_IGRAPHICSSERVICE_H


#include "IPixelShader.h"
#include "IVertexShader.h"
#include "internal/ITexture.h"
#include "IBitmapTexture.h"
#include "internal/IRenderTarget.h"
#include "IRenderTargetTexture.h"
#include "IDepthStencil.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "IScreenRenderTarget.h"
#include "IShaderProgram.h"

namespace mini3d
{
class IGraphicsService
{
public:

	// initialisation
	virtual ~IGraphicsService(void) { };

	// Properties
	virtual bool GetIsFullScreen() = 0;
	virtual void SetIsFullScreen(bool fullscreen) = 0;

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures() = 0;
	virtual int GetMaxTextureSize() = 0;
	virtual int GetPixelShaderVersion() = 0;
	virtual int GetVertexShaderVersion() = 0;

	// Create Resources
	virtual IScreenRenderTarget* CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IScreenRenderTarget::Quality quality) = 0;
	virtual IRenderTargetTexture* CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled) = 0;
	virtual IBitmapTexture* CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE) = 0;
	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration) = 0;
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices, unsigned int count, const IIndexBuffer::CullMode cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE) = 0;
	virtual IPixelShader* CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes) = 0;
	virtual IVertexShader* CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration) = 0;
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader) = 0;

	// Pipeline states

	virtual IShaderProgram* GetShaderProgram(void) = 0;
	virtual void SetShaderProgram(IShaderProgram* pShaderProgram) = 0;
	
	//virtual IPixelShader* GetPixelShader(void) = 0;
	//virtual void SetPixelShader(IPixelShader* pPixelShader) = 0;

	//virtual IVertexShader* GetVertexShader(void) = 0;
	//virtual void SetVertexShader(IVertexShader* pVertexShader) = 0;

	virtual ITexture* GetTexture(unsigned int index) = 0;
	virtual void SetTexture(ITexture* pTexture, unsigned int index) = 0;

	virtual IRenderTarget* GetRenderTarget(void) = 0;
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget) = 0;

	virtual IIndexBuffer* GetIndexBuffer(void) = 0;
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer) = 0;

	virtual IVertexBuffer* GetVertexBuffer(void) = 0;
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

	// Shader Parameters
	virtual void SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count) = 0;
	virtual void SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count) = 0;
	virtual void SetShaderParameterMatrix(unsigned int index, const float* pData, unsigned int rows, unsigned int columns) = 0;

	// Drawing
	virtual void Clear(int color) = 0;
	virtual void Draw(void) = 0;
	virtual void DrawIndices(unsigned int startIndex, unsigned int numIndices) = 0;

};
}

#endif
