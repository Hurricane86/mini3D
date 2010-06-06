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
#include "IFullscreenRenderTarget.h"
#include "IShaderProgram.h"

namespace mini3d
{
class IGraphicsService
{
public:

	// initialisation
	virtual ~IGraphicsService() { };

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures() const = 0;
	virtual int GetMaxTextureSize() const = 0;
	virtual int GetPixelShaderVersion() const = 0;
	virtual int GetVertexShaderVersion() const = 0;

	// Create Resources
	virtual IScreenRenderTarget* CreateScreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const IScreenRenderTarget::Quality& quality) = 0;
	virtual IFullscreenRenderTarget* CreateFullscreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& hWindow, const bool& depthTestEnabled, const IFullscreenRenderTarget::Quality& quality) = 0;
	virtual IRenderTargetTexture* CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) = 0;
	virtual IBitmapTexture* CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE) = 0;
	virtual IVertexBuffer* CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes) = 0;
	virtual IIndexBuffer* CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType = IIndexBuffer::INT_16, const IIndexBuffer::CullMode& cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE) = 0;
	virtual IPixelShader* CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes) = 0;
	virtual IVertexShader* CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount) = 0;
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader) = 0;

	// Pipeline states

	virtual IShaderProgram* GetShaderProgram() const = 0;
	virtual void SetShaderProgram(IShaderProgram* pShaderProgram) = 0;
	
	virtual ITexture* GetTexture(const unsigned int& index) const = 0;
	virtual void SetTexture(ITexture* pTexture, const unsigned int& index) = 0;

	virtual IRenderTarget* GetRenderTarget() const = 0;
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget) = 0;

	virtual IIndexBuffer* GetIndexBuffer() const = 0;
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer) = 0;

	virtual IVertexBuffer* GetVertexBuffer() const = 0;
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

	// Shader Parameters
	virtual void SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count) = 0;
	virtual void SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count) = 0;
	virtual void SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns) = 0;

	// Drawing
	virtual void Clear(const int& color) = 0;
	virtual void Draw() = 0;
	virtual void DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices) = 0;

};
}

#endif
