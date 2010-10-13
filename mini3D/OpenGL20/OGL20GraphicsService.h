
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


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
	IWindowRenderTarget* pCurrentWindowRenderTarget; // Keeps track of the windowrendertarget containing the current bound devicecontext (this is OpenGL specific)
	IDepthStencil* pCurrentDepthStencil;
	IVertexBuffer* pCurrentVertexBuffer;
	IIndexBuffer* pCurrentIndexBuffer;
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
	OGL20WindowRenderTarget::ScreenState isFullscreen;

	// number of currently set vertex attributes
	unsigned int numVertexAttributes;

public:
	

	// CONSTRUCTOR ------------------------------------------------------------

	OGL20GraphicsService();
	~OGL20GraphicsService();


	// IGRAPHICS SERVICE INTERFACE --------------------------------------------

	// Properties
	virtual OGL20WindowRenderTarget::ScreenState GetIsFullScreen() { return isFullscreen; };
	virtual void SetIsFullScreen(OGL20WindowRenderTarget::ScreenState value) { isFullscreen = value; };
	
	virtual HGLRC GetRenderContext() const { return hRenderContext; };
	virtual void SetRenderContext(HGLRC hRenderContext) { this->hRenderContext = hRenderContext; };	

	// Get Graphics Card Capabilities
	virtual unsigned int GetMaxTextures() const;
	virtual unsigned int GetMaxTextureSize() const;
	virtual unsigned int GetPixelShaderVersion() const;
	virtual unsigned int GetVertexShaderVersion() const;

	// Create Resources
	virtual IWindowRenderTarget* CreateWindowRenderTarget(const int& hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality);
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
	
	virtual IRenderTarget* GetRenderTarget() const { return pCurrentRenderTarget; }
	virtual void SetRenderTarget(IRenderTarget* pRenderTarget);

	// get the windowrendertarget containing the current bound devicecontext (this is OpenGL specific)
	inline IWindowRenderTarget* GetWindowRenderTarget() const {return pCurrentWindowRenderTarget;}
	
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

	IOS* GetOS() { return pOS; };

	IWindowRenderTarget* GetScreenRenderTarget() { return pCurrentWindowRenderTarget; };

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
