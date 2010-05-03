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
#include "OGL20ScreenRenderTarget.h"


namespace mini3d
{

class OGL20GraphicsService : public IGraphicsService
{
	// friends
friend class OGL20BitmapTexture;
friend class OGL20RenderTargetTexture;
friend class OGL20PixelShader;
friend class OGL20VertexShader;
friend class OGL20ShaderProgram;
friend class OGL20VertexBuffer;
friend class OGL20IndexBuffer;
friend class OGL20DepthStencil;
friend class OGL20ScreenRenderTarget;

private:
	
	// These member variables are accessed by the friend classes
	typedef std::set<IOGL20Resource*> ResourceContainer;
	ResourceContainer resourceList;

	// Member variables
	
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

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	OGL20GraphicsService(bool isFullscreen);
	~OGL20GraphicsService(void);


	// IGRAPHICS SERVICE INTERFACE --------------------------------------------

	// Properties
	virtual bool GetIsFullScreen()
	{ 
		return isFullscreen; 
	};
	virtual void SetIsFullScreen(bool value)
	{
		isFullscreen = value;
	};

	// Get Graphics Card Capabilities
	virtual int GetMaxTextures(void);
	virtual int GetMaxTextureSize(void);
	virtual int GetPixelShaderVersion(void);
	virtual int GetVertexShaderVersion(void);

	// Create Resources
	virtual IScreenRenderTarget* CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IScreenRenderTarget::Quality quality);
	virtual IRenderTargetTexture* CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled);
	virtual IBitmapTexture* CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE);
	virtual IVertexBuffer* CreateVertexBuffer(void* pVertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration);
	virtual IIndexBuffer* CreateIndexBuffer(int* pIndices,  unsigned int count, const IIndexBuffer::CullMode cullMode = IIndexBuffer::CULL_COUNTERCLOCKWIZE);
	virtual IPixelShader* CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes);
	virtual IVertexShader* CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration);
	virtual IShaderProgram* CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader);

	// Pipeline States
	//virtual IPixelShader* GetPixelShader(void);
	//virtual void SetPixelShader(IPixelShader* pPixelShader);
	//
	//virtual IVertexShader* GetVertexShader(void);
	//virtual void SetVertexShader(IVertexShader* pVertexShader);

	virtual IShaderProgram* GetShaderProgram(void);
	virtual void SetShaderProgram(IShaderProgram* pShaderProgram);

	virtual ITexture* GetTexture(unsigned int index);
	virtual void SetTexture(ITexture* pTexture, unsigned int index);
	
	virtual IRenderTarget* GetRenderTarget(void);
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget);
	
	virtual IDepthStencil* GetDepthStencil(void);
	virtual void SetDepthStencil(IDepthStencil* pDepthStencil);

	// Shader Parameters
	virtual void SetShaderParameterMatrix(unsigned int index, const float* pData, unsigned int rows, unsigned int columns);
	virtual void SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count);
	virtual void SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count);
	virtual void SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count);
	
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
	
	void printLog(GLuint obj);

	// Device creation
	void CreateDevice(void);
	void CreateInternalWindow(void);

	// Resource Management
	void AddResource(IOGL20Resource* resource);
	void RemoveResource(IOGL20Resource* resource);

	// Drawing Graphics
	virtual void BeginScene(void);
	virtual void EndScene(void);
	virtual void SetRenderStates(void);
	
};
}

#endif
