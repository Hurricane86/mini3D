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

#include "../OGL20GraphicsService.h"
#include "../os/windows/OSWindows.h"
#include "../../error/Error.h"
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/wglext.h>


// Constructor Destructor -----------------------------------------------------

mini3d::OGL20GraphicsService::OGL20GraphicsService() :
	isFullscreen(isFullscreen),
	pCurrentDepthStencil(0), isDrawingScene(false), deviceLost(true), lostDeviceCurrentITextures(0), currentITextures(0), pCurrentShaderProgram(0), pCurrentWindowRenderTarget(0)
{
	pOS = new OSWindows();
	CreateInternalWindow();

	// Get the device context for the window
	hDeviceContext = GetDC(hWindow);

	// Create the renderContext
	CreateDevice();
}

mini3d::OGL20GraphicsService::~OGL20GraphicsService(void)
{
	// Remove the default device context
	DeleteDC(hDeviceContext);

	// Clear the device/render context setting
    wglMakeCurrent(0, 0);

	// delete the default render context
	wglDeleteContext(hRenderContext);

	delete pOS;
}


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

// Locking resources
void mini3d::OGL20GraphicsService::BeginScene(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
}
void mini3d::OGL20GraphicsService::EndScene(void)
{

}

// setting render states
void mini3d::OGL20GraphicsService::SetRenderStates()
{

}


// Graphics Pipeline States ---------------------------------------------------

int mini3d::OGL20GraphicsService::GetMaxTextures() const
{
	GLint units;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);

	return units;
}
int mini3d::OGL20GraphicsService::GetMaxTextureSize() const
{
	GLint units;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &units);

	return units;
}
int mini3d::OGL20GraphicsService::GetPixelShaderVersion() const
{
	return 0;
}
int mini3d::OGL20GraphicsService::GetVertexShaderVersion() const
{
	return 0;
}


// Graphics Pipeline States ---------------------------------------------------

void mini3d::OGL20GraphicsService::printLog(GLuint obj)
{
	PFNGLISSHADERPROC glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");

	PFNGLGETSHADERIVPROC glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	PFNGLGETPROGRAMIVNVPROC glGetProgramiv = (PFNGLGETPROGRAMIVNVPROC)wglGetProcAddress("glGetProgramiv");
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	
	int infologLength = 0;
	int maxLength;
	
	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
			
	char* infoLog = new char[maxLength];
	LPCWSTR* infoLogW = new LPCWSTR[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
 
	OutputDebugStringA("\nDEBUG INFO ---------\n");

	if (infologLength > 0)
		OutputDebugStringA(infoLog);

	delete [] infoLog;
}

mini3d::IShaderProgram* mini3d::OGL20GraphicsService::GetShaderProgram(void) const
{
	return pCurrentShaderProgram;
}
void mini3d::OGL20GraphicsService::SetShaderProgram(IShaderProgram* pShaderProgram)
{
	
	if (pCurrentShaderProgram == pShaderProgram)
		return;

	PFNGLUSEPROGRAMPROC glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");

	if (pShaderProgram == 0)
	{
		glUseProgram(0);
	}
	else
	{
		OGL20ShaderProgram* pOLG20ShaderProgram = (OGL20ShaderProgram*)pShaderProgram;
		glUseProgram(pOLG20ShaderProgram->GetProgram());
	
		PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

		// Set the vertexattributes
		unsigned int vertexAttributeCount;
		OGL20VertexAttribute* vA = ((OGL20VertexShader*)(pOLG20ShaderProgram->GetVertexShader()))->GetVertexAttributes(vertexAttributeCount);

		for(unsigned int i = 0; i < vertexAttributeCount; i++)
		{
			glVertexAttribPointer(vA[i].index, vA[i].size, vA[i].type, vA[i].normalized, vA[i].stride, vA[i].pointer);
			glEnableVertexAttribArray(vA[i].index);
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

	PFNGLACTIVETEXTUREPROC glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	// if texture already assigned, then there is no need to re-assign it
	//if (pTexture == currentITextures[index])
	//	return;

	glActiveTexture(GL_TEXTURE0 + index);

	if (pTexture == 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
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
		glBindTexture(GL_TEXTURE_2D, pOGL20Texture->GetTextureBuffer());
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	hCurrentTexture = pTexture;
//	currentITextures[index] = pTexture;
}

// Render Target
void mini3d::OGL20GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{

	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

	if (pRenderTarget == 0)
	{
		// early out if there is no change
		if (pRenderTarget == pCurrentRenderTarget)
			return;
		
		// set the frame buffer 0 and the device/render context to the default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		wglMakeCurrent(hDeviceContext, hRenderContext);
		pCurrentRenderTarget = 0;
		return;
	}

	// This is a dynamic cast used as a typecheck, code police says this should be solved with virtual function calls instead
	OGL20RenderTargetTexture* pOGL20RenderTargetTexture = dynamic_cast<OGL20RenderTargetTexture*>(pRenderTarget);

	if (pOGL20RenderTargetTexture != 0)
	{
		if (pRenderTarget == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pOGL20RenderTargetTexture->GetRenderTargetBuffer());
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
		if ((pCurrentWindowRenderTarget == (IWindowRenderTarget*)pOGL20WindowRenderTarget) && (isFullscreen == pOGL20WindowRenderTarget->GetScreenState()))
			return;
		
		if (pOGL20WindowRenderTarget->GetScreenState() == OGL20WindowRenderTarget::SCREEN_STATE_WINDOWED)
		{
			// TODO: Restore screen resolution if necessary

			ChangeDisplaySettings(NULL,0);	// Switch back to the desktop default resolution stored in registry
			ShowCursor(TRUE);	// Show mouse pointer
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			wglMakeCurrent(pOGL20WindowRenderTarget->GetDeviceContext(), hRenderContext);
			glViewport(0,0,pOGL20WindowRenderTarget->GetWidth(), pOGL20WindowRenderTarget->GetHeight());
		}
		else if (pOGL20WindowRenderTarget->GetScreenState() == OGL20WindowRenderTarget::SCREEN_STATE_FULLSCREEN)
		{
			
			// Set the video resolution to the fullscreen resolution

			DEVMODE dmScreenSettings = {0}; // Device Mode initialized to zero
			dmScreenSettings.dmSize=sizeof(dmScreenSettings); // Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth = pOGL20WindowRenderTarget->GetFullscreenWidth(); // Selected Screen Width
			dmScreenSettings.dmPelsHeight = pOGL20WindowRenderTarget->GetFullscreenHeight();	// Selected Screen Height

			// if the size is zero, use the default desktop size
			if (dmScreenSettings.dmPelsWidth != 0 && dmScreenSettings.dmPelsWidth != 0)
			{
				dmScreenSettings.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;
			}

			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				return;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			wglMakeCurrent(pOGL20WindowRenderTarget->GetDeviceContext(), hRenderContext);
			glViewport(0,0,pOGL20WindowRenderTarget->GetFullscreenWidth(), pOGL20WindowRenderTarget->GetFullscreenHeight());
		}

		// set the current render target to this
		pCurrentRenderTarget = pRenderTarget;
		
		// Because render target textures overrides window render targets we need to store this one also.
		pCurrentWindowRenderTarget = pOGL20WindowRenderTarget;

		// Set the current fullscreen state (to keep track if the user sets the screen state of the current render target)
		SetIsFullScreen(pOGL20WindowRenderTarget->GetScreenState());

		return;
	}

}

// DepthStencil
mini3d::IDepthStencil* mini3d::OGL20GraphicsService::GetDepthStencil() const
{
	return pCurrentDepthStencil;
}
void mini3d::OGL20GraphicsService::SetDepthStencil(IDepthStencil* pDepthStencil)
{
	if (pDepthStencil == pCurrentDepthStencil)
		return;
	
	pCurrentDepthStencil = pDepthStencil;
}

// Index Buffer
mini3d::IIndexBuffer* mini3d::OGL20GraphicsService::GetIndexBuffer() const
{	
	return pCurrentIndexBuffer;
}
void mini3d::OGL20GraphicsService::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
{
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

	if (pCurrentIndexBuffer == pIndexBuffer)
		return;

	if (pIndexBuffer == 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		OGL20IndexBuffer* pOGL20IndexBuffer = (OGL20IndexBuffer*)pIndexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pOGL20IndexBuffer->GetIndexBuffer());
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
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

	if (pCurrentVertexBuffer == pVertexBuffer)
		return;

	if (pVertexBuffer == 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		OGL20VertexBuffer* pOGL20VertexBuffer = (OGL20VertexBuffer*)pVertexBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, pOGL20VertexBuffer->GetVertexBuffer());		
	}

	pCurrentVertexBuffer = pVertexBuffer;
}

// Shader Parameters
void mini3d::OGL20GraphicsService::SetShaderParameterMatrix(const unsigned int& index, const float* pData, const unsigned int& rows, const unsigned int& columns)
{
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	if (rows == 4 && columns == 4)
		glUniformMatrix4fv(index, 1, GL_FALSE, pData);
}
void mini3d::OGL20GraphicsService::SetShaderParameterFloat(const unsigned int& index, const float* pData, const unsigned int& count)
{
	PFNGLUNIFORM1FPROC glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	PFNGLUNIFORM2FPROC glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	PFNGLUNIFORM3FPROC glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	PFNGLUNIFORM4FPROC glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");

	if (count == 1)
		glUniform1f(index, *pData);
	else if (count == 2)
		glUniform2f(index, *pData, *(pData + 1));
	else if (count == 3)
		glUniform3f(index, *pData, *(pData + 1), *(pData + 2));
	else if (count == 4)
		glUniform4f(index, *pData, *(pData + 1), *(pData + 2), *(pData + 3));
}
void mini3d::OGL20GraphicsService::SetShaderParameterInt(const unsigned int& index, const int* pData, const unsigned int& count)
{
	
	PFNGLUNIFORM1IPROC glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	PFNGLUNIFORM2IPROC glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
	PFNGLUNIFORM3IPROC glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
	PFNGLUNIFORM4IPROC glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");

	if (count == 1)
		glUniform1i(index, *pData);
	else if (count == 2)
		glUniform2i(index, *pData, *(pData + 1));
	else if (count == 3)
		glUniform3i(index, *pData, *(pData + 1), *(pData + 2));
	else if (count == 4)
		glUniform4i(index, *pData, *(pData + 1), *(pData + 2), *(pData + 3));
}

// Drawing
void mini3d::OGL20GraphicsService::Draw()
{
	BeginScene();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glDrawElements(GL_TRIANGLES, pCurrentIndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

}
void mini3d::OGL20GraphicsService::DrawIndices(const unsigned int& startIndex, const unsigned int& numIndices)
{
	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");

	BeginScene();
	glDrawRangeElements(GL_TRIANGLES, startIndex, numIndices + startIndex, numIndices / 3, GL_UNSIGNED_INT, 0);
}

// Clear
void mini3d::OGL20GraphicsService::Clear(const float& r, const float& g, const float& b, const float& a)
{
	glClearColor(r, g, b, a);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Create Resources
mini3d::IWindowRenderTarget* mini3d::OGL20GraphicsService::CreateWindowRenderTarget(const int& hWindow, const bool& depthTestEnabled, const IWindowRenderTarget::Quality& quality)
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
