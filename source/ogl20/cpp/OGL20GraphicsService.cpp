
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20GraphicsService.h"
#include "../../mini3d/datatypes/error/Error.h"
#include "../../oswrapper/OSWrapper.h"


// Constructor Destructor -----------------------------------------------------

mini3d::OGL20GraphicsService::OGL20GraphicsService() :
	isFullscreen(isFullscreen),
	isDrawingScene(false), deviceLost(true), lostDeviceCurrentITextures(0), currentITextures(0), pCurrentShaderProgram(0), pCurrentWindowRenderTarget(0)
{
	pOSWrapper = new OSWrapper();
	//CreateInternalWindow();

	//// Get the device context for the window
	//hDeviceContext = GetDC(hWindow);

	//// Create the renderContext
	//CreateDevice();

	//// Should be able to init the pOGLWrapper connections
	//pOGLWrapper->Init();
}

mini3d::OGL20GraphicsService::~OGL20GraphicsService(void)
{
	// Remove the default device context
	//DeleteDC(hDeviceContext);

	// Clear the device/render context setting
    //wglMakeCurrent(0, 0);

	// delete the default render context
	//wglDeleteContext(hRenderContext);
}

/*
// Private helper methods -----------------------------------------------------

void mini3d::OGL20GraphicsService::CreateDevice(void)
{
	// get the pixel format for the device context
	// query desktop video settings
	DEVMODE SDL_desktop_mode;
	EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, &SDL_desktop_mode);
	
	PIXELFORMATDESCRIPTOR pfd={0};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Make the pixel format compatible with opengl
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
    
	// Get the pixel format index for the new pixel format
	int iNewFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat( hDeviceContext, iNewFormat, &pfd);

	// Create a default reder context and set it as current so we can start loading graphics data to the graphics card
	hRenderContext = wglCreateContext(hDeviceContext);
	wglMakeCurrent(hDeviceContext, hRenderContext);

}

LRESULT CALLBACK OGL20WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void mini3d::OGL20GraphicsService::CreateInternalWindow(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = OGL20WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"OGL20InternalWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
		hWindow = 0;
		return;
    }

	hWindow = CreateWindowEx(WS_EX_CLIENTEDGE, L"OGL20InternalWindowClass", L"HiddenWindow", 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
}
*/

// Locking resources
void mini3d::OGL20GraphicsService::BeginScene(void)
{
	mOGLWrapper.GLShadeModel(GL_SMOOTH);
	mOGLWrapper.GLEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	mOGLWrapper.GLDepthFunc(GL_LEQUAL);						// The Type Of Depth Test To Do
}
void mini3d::OGL20GraphicsService::EndScene(void)
{

}

// setting render states
void mini3d::OGL20GraphicsService::SetRenderStates()
{

}


// Graphics Pipeline States ---------------------------------------------------

unsigned int mini3d::OGL20GraphicsService::GetMaxTextures() const
{
	GLint units;
	mOGLWrapper.GLGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);

	return units;
}
unsigned int mini3d::OGL20GraphicsService::GetMaxTextureSize() const
{
	GLint units;
	mOGLWrapper.GLGetIntegerv(GL_MAX_TEXTURE_SIZE, &units);

	return units;
}
unsigned int mini3d::OGL20GraphicsService::GetPixelShaderVersion() const
{
	return 0;
}
unsigned int mini3d::OGL20GraphicsService::GetVertexShaderVersion() const
{
	return 0;
}


// Graphics Pipeline States ---------------------------------------------------

mini3d::IShaderProgram* mini3d::OGL20GraphicsService::GetShaderProgram(void) const
{
	return pCurrentShaderProgram;
}
void mini3d::OGL20GraphicsService::SetShaderProgram(IShaderProgram* pShaderProgram)
{
	
	if (pCurrentShaderProgram == pShaderProgram)
		return;

	if (pShaderProgram == 0)
	{
		mOGLWrapper.GLUseProgram(0);
	}
	else
	{
		OGL20ShaderProgram* pOLG20ShaderProgram = (OGL20ShaderProgram*)pShaderProgram;
		mOGLWrapper.GLUseProgram(pOLG20ShaderProgram->GetProgram());
	
		// Set the vertexattributes
		unsigned int vertexAttributeCount;
		OGL20VertexAttribute* vA = ((OGL20VertexShader*)(pOLG20ShaderProgram->GetVertexShader()))->GetVertexAttributes(vertexAttributeCount);

		for(unsigned int i = 0; i < vertexAttributeCount; i++)
		{
			mOGLWrapper.GLVertexAttribPointer(vA[i].index, vA[i].size, vA[i].type, vA[i].normalized, vA[i].stride, vA[i].pointer);
			mOGLWrapper.GLEnableVertexAttribArray(vA[i].index);
		}
	}

	pCurrentShaderProgram = pShaderProgram;
}


// Texture
mini3d::ITexture* mini3d::OGL20GraphicsService::GetTexture(const unsigned int& index) const
{
	if (index > GetMaxTextures())
	{
		// Throw error in debug mode;
		return 0;
	}

//	return currentITextures[index];
	return 0;
}
void mini3d::OGL20GraphicsService::SetTexture(ITexture* pTexture, const unsigned int& index)
{	

	if (index >  GetMaxTextures())
	{
		throw Error::MINI3D_ERROR_TEXTURE_INDEX_OUTSIDE_VALID_RANGE;
	}

	// if texture already assigned, then there is no need to re-assign it
	//if (pTexture == currentITextures[index])
	//	return;

	mOGLWrapper.GLActiveTexture(GL_TEXTURE0 + index);

	if (pTexture == 0)
	{
		mOGLWrapper.GLBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{

		// we always set the wrapstyle too... if we are changing the texture the overhead of also changing wrap style is small.

		switch(pTexture->GetWrapStyle())
		{
		case ITexture::WRAP_TILE:

			break;
		case ITexture::WRAP_MIRROR:

			break;
		case ITexture::WRAP_CLAMP:

			break;
		}

		// set the wrap style
	
		// set the texture
		// this cast is "unfailable" (not exception caught). Whoever inherits from ITexture must also inherit from IOGL20Texture
		IOGL20Texture* pOGL20Texture = dynamic_cast<IOGL20Texture*>(pTexture);
		mOGLWrapper.GLBindTexture(GL_TEXTURE_2D, pOGL20Texture->GetTextureBuffer());
	}

	mOGLWrapper.GLTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	mOGLWrapper.GLTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	hCurrentTexture = pTexture;
//	currentITextures[index] = pTexture;
}

// Render Target
void mini3d::OGL20GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{

	if (pRenderTarget == 0)
	{
		// early out if there is no change
		if (pRenderTarget == pCurrentRenderTarget)
			return;
		
		// set the frame buffer 0 and the device/render context to the default
		mOGLWrapper.GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		mOGLWrapper.SetDefaultRenderWindow();
		pCurrentRenderTarget = 0;
		return;
	}

	// This is a dynamic cast used as a typecheck, code police says this should be solved with virtual function calls instead
	OGL20RenderTargetTexture* pOGL20RenderTargetTexture = dynamic_cast<OGL20RenderTargetTexture*>(pRenderTarget);

	if (pOGL20RenderTargetTexture != 0)
	{
		if (pRenderTarget == 0)
		{
			mOGLWrapper.GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			mOGLWrapper.GLBindFramebuffer(GL_FRAMEBUFFER, pOGL20RenderTargetTexture->GetRenderTargetBuffer());
		}
		
		pCurrentRenderTarget = pRenderTarget;
		return;
	}

	// This is a dynamic cast used as a typecheck, code police says this should be solved with virtual function calls instead
	OGL20WindowRenderTarget* pOGL20WindowRenderTarget = dynamic_cast<OGL20WindowRenderTarget*>(pRenderTarget);

	if (pOGL20WindowRenderTarget != 0)
	{

		// TODO: Verify screen res also if switching fullscreen -> fullscreen (force set?)

		// if this rendertarget is already the current and the correct screen state is set, return
		if (pCurrentWindowRenderTarget == (IWindowRenderTarget*)pOGL20WindowRenderTarget)
			return;
		
		mOGLWrapper.SetRenderWindow(pOGL20WindowRenderTarget->GetWindowHandle());
		glViewport(0,0,pOGL20WindowRenderTarget->GetWidth(), pOGL20WindowRenderTarget->GetHeight());

		// set the current render target to this
		pCurrentRenderTarget = pRenderTarget;
		
		// Because render target textures overrides window render targets we need to store this one also.
		pCurrentWindowRenderTarget = pOGL20WindowRenderTarget;

		// Set the current fullscreen state (to keep track if the user sets the screen state of the current render target)
		SetIsFullScreen(pOGL20WindowRenderTarget->GetScreenState());

		return;
	}

}

// Index Buffer
mini3d::IIndexBuffer* mini3d::OGL20GraphicsService::GetIndexBuffer() const
{	
	return pCurrentIndexBuffer;
}
void mini3d::OGL20GraphicsService::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
{

	if (pCurrentIndexBuffer == pIndexBuffer)
		return;

	if (pIndexBuffer == 0)
	{
		mOGLWrapper.GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		OGL20IndexBuffer* pOGL20IndexBuffer = (OGL20IndexBuffer*)pIndexBuffer;
		mOGLWrapper.GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pOGL20IndexBuffer->GetIndexBuffer());
	}

	pCurrentIndexBuffer = pIndexBuffer;
}

// Vertex Buffer
mini3d::IVertexBuffer* mini3d::OGL20GraphicsService::GetVertexBuffer() const
{
	return pCurrentVertexBuffer;
}
void mini3d::OGL20GraphicsService::SetVertexBuffer(IVertexBuffer* pVertexBuffer)
{
	if (pCurrentVertexBuffer == pVertexBuffer)
		return;

	if (pVertexBuffer == 0)
	{
		mOGLWrapper.GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		OGL20VertexBuffer* pOGL20VertexBuffer = (OGL20VertexBuffer*)pVertexBuffer;
		mOGLWrapper.GLBindBuffer(GL_ARRAY_BUFFER, pOGL20VertexBuffer->GetVertexBuffer());		
	}

	pCurrentVertexBuffer = pVertexBuffer;
}

// Shader Parameters
void mini3d::OGL20GraphicsService::SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns)
{
	if (rows == 4 && columns == 4)
		mOGLWrapper.GLUniformMatrix4fv(index, 1, GL_FALSE, pData);
}
void mini3d::OGL20GraphicsService::SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count)
{
	if (count == 1)
		mOGLWrapper.GLUniform1f(index, *pData);
	else if (count == 2)
		mOGLWrapper.GLUniform2f(index, *pData, *(pData + 1));
	else if (count == 3)
		mOGLWrapper.GLUniform3f(index, *pData, *(pData + 1), *(pData + 2));
	else if (count == 4)
		mOGLWrapper.GLUniform4f(index, *pData, *(pData + 1), *(pData + 2), *(pData + 3));
}
void mini3d::OGL20GraphicsService::SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count)
{
	if (count == 1)
		mOGLWrapper.GLUniform1i(index, *pData);
	else if (count == 2)
		mOGLWrapper.GLUniform2i(index, *pData, *(pData + 1));
	else if (count == 3)
		mOGLWrapper.GLUniform3i(index, *pData, *(pData + 1), *(pData + 2));
	else if (count == 4)
		mOGLWrapper.GLUniform4i(index, *pData, *(pData + 1), *(pData + 2), *(pData + 3));
}

// Drawing
void mini3d::OGL20GraphicsService::Draw()
{
	BeginScene();

	mOGLWrapper.GLEnableClientState(GL_VERTEX_ARRAY);
	mOGLWrapper.GLEnableClientState(GL_INDEX_ARRAY);
	mOGLWrapper.GLDrawElements(GL_TRIANGLES, pCurrentIndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, 0);
	mOGLWrapper.GLDisableClientState(GL_VERTEX_ARRAY);
	mOGLWrapper.GLDisableClientState(GL_INDEX_ARRAY);

}
void mini3d::OGL20GraphicsService::DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices)
{
	BeginScene();
	mOGLWrapper.GLDrawRangeElements(GL_TRIANGLES, startIndex, numIndices + startIndex, numIndices / 3, GL_UNSIGNED_INT, 0);
}

// Clear
void mini3d::OGL20GraphicsService::Clear(const float& r, const float& g, const float& b, const float& a)
{
	mOGLWrapper.GLClearColor(r, g, b, a);
	mOGLWrapper.GLClearDepth(1.0f);
	mOGLWrapper.GLClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Create Resources
mini3d::IWindowRenderTarget* mini3d::OGL20GraphicsService::CreateWindowRenderTarget(const MINI3D_WINDOW hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality)
{
	return new OGL20WindowRenderTarget(this, hWindow, depthTestEnabled, quality);
}
mini3d::IRenderTargetTexture* mini3d::OGL20GraphicsService::CreateRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{
	return new OGL20RenderTargetTexture(this, width, height, depthTestEnabled);
}
mini3d::IBitmapTexture* mini3d::OGL20GraphicsService::CreateBitmapTexture(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle)
{
	return new OGL20BitmapTexture(this, pBitmap, width, height, bitDepth, wrapStyle);
}
mini3d::IVertexBuffer* mini3d::OGL20GraphicsService::CreateVertexBuffer(const void* pVertices,const  unsigned int& count, const unsigned int& vertexSizeInBytes)
{
	return new OGL20VertexBuffer(this, pVertices, count, vertexSizeInBytes);
}
mini3d::IIndexBuffer* mini3d::OGL20GraphicsService::CreateIndexBuffer(const void* pIndices, const unsigned int& count, const IIndexBuffer::DataType& dataType, const IIndexBuffer::CullMode& cullMode)
{
	return new OGL20IndexBuffer(this, pIndices, count, dataType, cullMode);
}
mini3d::IPixelShader* mini3d::OGL20GraphicsService::CreatePixelShader(const void* shaderBytes, const unsigned int& sizeInBytes)
{
	return new OGL20PixelShader(this, shaderBytes, sizeInBytes);
}
mini3d::IVertexShader* mini3d::OGL20GraphicsService::CreateVertexShader(const void* shaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount)
{
	return new OGL20VertexShader(this, shaderBytes, sizeInBytes, vertexDeclaration, vertexDataCount);
}
mini3d::IShaderProgram* mini3d::OGL20GraphicsService::CreateShaderProgram(IVertexShader* pVertexShader, IPixelShader* pPixelShader)
{
	return new OGL20ShaderProgram(this, pVertexShader, pPixelShader);
}


// Resource Management
void mini3d::OGL20GraphicsService::AddResource(IOGL20Resource* resource)
{
	resourceList.insert(resource);
}
void mini3d::OGL20GraphicsService::RemoveResource(IOGL20Resource* resource)
{
	resourceList.erase(resource);
}
