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

#ifndef AURORA_OGL20GRAPHICSSERVICE_H
#define AURORA_OGL20GRAPHICSSERVICE_H

#include <windows.h>
#include <GL/gl.h>
#include <set>
#include <map>
#include <string>
#include "internal/IOGL20Resource.h"
#include "../IGraphicsService.h"

#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
#endif

#include "internal/IOGL20RenderTarget.h"
#include "internal/IOGL20Texture.h"
#include "internal/IOGL20Resource.h"

#include "OGL20BitmapTexture.h"
#include "OGL20IndexBuffer.h"
#include "OGL20PixelShader.h"
#include "OGL20ShaderProgram.h"
#include "OGL20VertexShader.h"
#include "OGL20VertexBuffer.h"
#include "OGL20IndexBuffer.h"
#include "OGL20RenderTargetTexture.h"
#include "OGL20WindowRenderTarget.h"
#include "OGL20FullscreenRenderTarget.h"
#include "os\IOS.h"

namespace mini3d
{

class OGL20GraphicsService : public IGraphicsService
{

private:
	
	// These member variables are accessed by the friend classes
	typedef std::set<IOGL20Resource*> ResourceContainer;
	ResourceContainer resourceList;

	// Member variables
	
	// Operating system functions
	IOS* pOS;

	// currently loaded resources
	IRenderTarget* pCurrentRenderTarget;
	IDepthStencil* pCurrentDepthStencil;
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;
	//IPixelShader* pCurrentPixelShader;
	//IVertexShader* pCurrentVertexShader;
	IShaderProgram* pCurrentShaderProgram;
	ITexture** currentITextures;

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
	HDC hDeviceContext;
	HGLRC hRenderContext;
	GLuint hProgram;
	ITexture* hCurrentTexture;
	bool deviceLost;
	bool isFullscreen;

	// number of currently set vertex attributes
	unsigned int numVertexAttributes;

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	OGL20GraphicsService();
	~OGL20GraphicsService();


	// IGRAPHICS SERVICE INTERFACE --------------------------------------------

	// Properties
	virtual bool GetIsFullScreen() { return isFullscreen; };
	virtual void SetIsFullScreen(bool value) { isFullscreen = value; };
	
	virtual HGLRC GetRenderContext() const { return hRenderContext; };
	virtual void SetRenderContext(HGLRC hRenderContext) { this->hRenderContext = hRenderContext; };	

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures() const;
	virtual int GetMaxTextureSize() const;
	virtual int GetPixelShaderVersion() const;
	virtual int GetVertexShaderVersion() const;

	// Create Resources
	virtual IWindowRenderTarget* CreateWindowRenderTarget(const unsigned int& width, const unsigned int& height, const int& hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality);
	virtual IFullscreenRenderTarget* CreateFullscreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& hWindow, const bool& depthTestEnabled, const IFullscreenRenderTarget::Quality& quality);
	virtual IRenderTargetTexture* CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled);
	virtual IBitmapTexture* CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	virtual IVertexBuffer* CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes);
	virtual IIndexBuffer* CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType = IIndexBuffer::INT_32, const IIndexBuffer::CullMode& cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE);
	virtual IPixelShader* CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes);
	virtual IVertexShader* CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader);

	// Pipeline States
	//virtual IPixelShader* GetPixelShader() const;
	//virtual void SetPixelShader(IPixelShader* pPixelShader);
	
	//virtual IVertexShader* GetVertexShader() const;
	//virtual void SetVertexShader(IVertexShader* pVertexShader);

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

	
	// Resource Management
	void AddResource(IOGL20Resource* resource);
	void RemoveResource(IOGL20Resource* resource);


	// PUBLIC MEMBER FUNCTIONS ------------------------------------------------

	IOS* GetOS() { return pOS; }

private:
	
	// INTERNAL HELPER FUNCTIONS ----------------------------------------------
	
	void printLog(GLuint obj);

	// Device creation
	void CreateDevice();
	void CreateInternalWindow();

	// Drawing Graphics
	virtual void BeginScene();
	virtual void EndScene();
	virtual void SetRenderStates();
	
};
}

#endif
