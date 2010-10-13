
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IGRAPHICSSERVICE_H
#define MINI3D_IGRAPHICSSERVICE_H


#include "IPixelShader.h"
#include "IVertexShader.h"
#include "internal/ITexture.h"
#include "IBitmapTexture.h"
#include "internal/IRenderTarget.h"
#include "IRenderTargetTexture.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "IWindowRenderTarget.h"
#include "IShaderProgram.h"

namespace mini3d
{
/**
 * IGraphcisService is the hub of a mini3d implementation
 * 
 * It contains functionality to control and configure the graphics hardware.
 * The basic functionality consists of:
 *
 * - Creating Resources (textures, geometry, shaders and so on...)
 *
 * - Configuring the render pipeline by setting the resources to be used when rendering.
 *
 * - Finding the capabilities of the hardware.
 */
class IGraphicsService
{
public:

	// initialisation
	virtual ~IGraphicsService() { };

	// Get Graphics Card Capabilities
	virtual unsigned int GetMaxTextures() const = 0;
	virtual unsigned int GetMaxTextureSize() const = 0;
	virtual unsigned int GetPixelShaderVersion() const = 0;
	virtual unsigned int GetVertexShaderVersion() const = 0;

	// Create Resources

/**
	Creates a compatible IWindowRenderTarget
	 
	@param windowHandle Operating system specific handle to the widget that shall define the region
	of the render target. This could be a window or a graphical component depending on the implementaion
	and the operating system used.
	@param depthTestEnabled Determine if the graphics service should use a depth buffer when rendering
	to the render target. Enabeling depth testing hides geometry that is hidden behind other geometry.
	If depth testing is disabled triangles will draw on top of each other depending on the order in which
	they are drawn and not depending on their depth.
	@param quality The quality level that should be used for drawing.
	@see IWindowRenderTarget
	@see IWindowRenderTarget::Quality
*/
	virtual IWindowRenderTarget* CreateWindowRenderTarget(const int& windowHandle, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality) = 0;

/**
	Creates a compatible IRenderTargetTexture
	
	@param width Width of the render target texture.
	@param height Height of the render target texture.
	@param depthTestEnabled Determine if the graphics service should use a depth buffer when rendering
	to the render target. Enabeling depth testing hides geometry that is hidden behind other geometry.
	If depth testing is disabled triangles will draw on top of each other depending on the order in which
	they are drawn and not depending on their depth.
	@see IRenderTargetTexture
*/
	virtual IRenderTargetTexture* CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) = 0;

/**
	Creates a compatible IBitmapTexture
	
	@param pBitmap Pointer to a buffer containing the bitmap data for creating the texture.
	@param width Width of the bitmap described by the bitmap data.
	@param height Height of the bitmap described by the bimap data.
	@param bitDepth Number of bits per pixel for the bitmap data.
	@param wrapStyle How the texture will be repeated if it is tiled.
	@see IBitmapTexture
*/
	virtual IBitmapTexture* CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE) = 0;

/**
	Creates a compatible IVertexBuffer
	
	@param pVertice Pointer to a buffer containing the vertex data for creating the buffer.
	@param count Number of vertices in the pVertices buffer.
	@param vertexSizeInBytes Size of a vertex in the buffer in bytes
	@see IVertexBuffer
*/
	virtual IVertexBuffer* CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes) = 0;

/**
	Creates a compatible IIndexBuffer
	
	@param pIndices Pointer to a buffer containing the index data for creating the buffer.
	@param count Number of indices in the pIndices buffer.
	@param dataType Data type for the indices (how many bits per index), default is 32 bits per index.
	@param cullMode Cull mode to be used when drawing geometry using this IIndexBuffer, default is counter clockwize
	@see IIndexBuffer
	@see IIndexBuffer::DataType
	@see IIndexBuffer::CullMode
*/
	virtual IIndexBuffer* CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType = IIndexBuffer::INT_32, const IIndexBuffer::CullMode& cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE) = 0;

/**
	Creates a compatible IPixelShader
	
	@param shaderBytes Pointer to a buffer containing the data for creating the shader.
	@param sizeInBytes Size of the shaderBytes buffer in bytes.
	@see IPixelShader
*/
	virtual IPixelShader* CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes) = 0;

/**
	Creates a compatible IVertexShader
	
	@param shaderBytes Pointer to a buffer containing the data for creating the shader.
	@param sizeInBytes Size of the shaderBytes buffer in bytes.
	@param vertexDeclaration Array describing the contents of the shaderBytes buffer.
	@param vertexDataCount Number of elements in the vertexDeclaration array.
	@see IVertexShader
*/
	virtual IVertexShader* CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount) = 0;

/**
	Creates a compatible IShaderProgram
	
	@param pVertexShader Pointer vertex shader.
	@param pPixelShader Pointer to a pixel shader.
	@see IShaderProgram
*/
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader) = 0;


// Pipeline states

/**
	Get the currently bound shader program.

	@return The currently bound shader program
	@see IShaderProgram
*/
	virtual IShaderProgram* GetShaderProgram() const = 0;

/**
	Sets the currently bound shader program.

	@return The currently bound shader program
	@see IShaderProgram
*/
	virtual void SetShaderProgram(IShaderProgram* pShaderProgram) = 0;

/**
	Gets the currently bound texture at a specific texture index.
	@param index Index of the texture to get.	
	@return The currently bound texture at the given index
	@see ITexture
*/
	virtual ITexture* GetTexture(const unsigned int& index) const = 0;

/**
	Sets the currently bound texture at a specific texture index.
	@param pTexture Texture to bind.
	@param index Index of the texture to set.	
	@see ITexture
*/
	virtual void SetTexture(ITexture* pTexture, const unsigned int& index) = 0;

/**
	Gets the currently bound render target.
	@return Currently bound render target.
	@see IRenderTarget
*/
	virtual IRenderTarget* GetRenderTarget() const = 0;
/**
	Sets the currently bound render target.
	@param pRenderTarget Render target to bind.
	@see IRenderTarget
*/
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget) = 0;

/**
	Gets the currently bound index buffer.
	@return Currently bound index buffer.
	@see IIndexBuffer
*/
	virtual IIndexBuffer* GetIndexBuffer() const = 0;

/**
	Sets the currently bound index buffer.
	@param indexBuffer Index buffer to bind.
	@see IIndexBuffer
*/
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer) = 0;

/**
	Gets the currently bound vertex buffer.
	@return Currently bound vertex buffer.
	@see IVertexBuffer
*/
	virtual IVertexBuffer* GetVertexBuffer() const = 0;

/**
	Sets the currently bound vertex buffer.
	@param vertexBuffer Vertex buffer to bind.
	@see IVertexBuffer
*/
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

	// Shader Parameters

/**
	Copies a number of floats to the data register that is accessible from shaders.
	@param index Parameter index to set
	@param pData The data to write to the index
	@param count The number of floats to copy from pData to the shader parameter buffer
*/
	virtual void SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count) = 0;

/**
	Copies a number of ints to the data register that is accessible from shaders.
	@param index Parameter index to set
	@param pData The data to write to the index
	@param count The number of ints to copy from pData to the shader parameter buffer
*/
	virtual void SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count) = 0;

/**
	Copies a matrix of floats to the data register that is accessible from shaders.
	@param index Parameter index to set
	@param pData The data to write to the index
	@param count The number of ints to copy from pData to the shader parameter buffer
*/
	virtual void SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns) = 0;

// Drawing
/**
	Clear the currently bound render target
	@param r the red component of the color to clear the render target with.
	@param g the green component of the color to clear the render target with.
	@param b the blue component of the color to clear the render target with.
	@param a the alpha component of the color to clear the render target with.
*/
	virtual void Clear(const float& r, const float& g, const float& b, const float& a) = 0;

/**
	Use the currently bound resources to draw to the currently bound rendertarget
*/
	virtual void Draw() = 0;

/**
	Use the currently bound resources and a selected set of indices from the index buffer to
	draw to the currently bound rendertarget.
	@param startIndex Index of the first vertex index to draw.
	@param numIndex The number of vertex indices to draw.
*/
	virtual void DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices) = 0;
};
}

#endif
