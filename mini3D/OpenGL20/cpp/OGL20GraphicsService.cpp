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
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/wglext.h>

// Constructor Destructor -----------------------------------------------------

mini3d::OGL20GraphicsService::OGL20GraphicsService(bool isFullscreen) :
	isFullscreen(isFullscreen),
	pCurrentDepthStencil(0), isDrawingScene(false), deviceLost(true), lostDeviceCurrentITextures(0), currentITextures(0), pCurrentShaderProgram(0)
{
	CreateInternalWindow();
	CreateDevice();
}
mini3d::OGL20GraphicsService::~OGL20GraphicsService(void)
{
    wglMakeCurrent( NULL, NULL );
}



// Private helper methods -----------------------------------------------------

void mini3d::OGL20GraphicsService::CreateDevice(void)
{
    HDC hDeviceContext = GetDC(hWindow);
	
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	// set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16; // TODO: 16 vs 32 depth shizzle, get info from graphicsService
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	if (!SetPixelFormat(hDeviceContext, iFormat, &pfd))
	{
		int i = 0;
	}
	
	hRenderContext=wglCreateContext(hDeviceContext);
	
	if (!wglMakeCurrent(hDeviceContext, hRenderContext))
	{
		int i = 0;
	}

	// create the shader program
	PFNGLCREATEPROGRAMPROC glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	PFNGLUSEPROGRAMPROC glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");

	hProgram = glCreateProgram();
	glUseProgram(hProgram);

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

}
void mini3d::OGL20GraphicsService::EndScene(void)
{

}

// setting render states
void mini3d::OGL20GraphicsService::SetRenderStates()
{

}



// Graphics Pipeline States ---------------------------------------------------

int mini3d::OGL20GraphicsService::GetMaxTextures()
{
	GLint units;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);

	return units;
}
int mini3d::OGL20GraphicsService::GetMaxTextureSize()
{
	GLint units;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &units);

	return units;
}
int mini3d::OGL20GraphicsService::GetPixelShaderVersion()
{
	return 0;
}
int mini3d::OGL20GraphicsService::GetVertexShaderVersion()
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

mini3d::IShaderProgram* mini3d::OGL20GraphicsService::GetShaderProgram(void)
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
	}

	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	
	// Set the vertexattributes

	// todo: clean up
	OGL20ShaderProgram* pOLG20ShaderProgram = (OGL20ShaderProgram*)pShaderProgram;
	OGL20VertexShader::VertexAttributeVector vA = ((OGL20VertexShader*)(pOLG20ShaderProgram->pVertexShader))->GetVertexAttributes();

	for(int i = 0; i < vA.size(); i++)
	{
		glVertexAttribPointer(vA[i].index, vA[i].size, vA[i].type, vA[i].normalized, vA[i].stride, vA[i].pointer);
		glEnableVertexAttribArray(vA[i].index);
	}

	pCurrentShaderProgram = pShaderProgram;
}

// Pixlel Shader
//mini3d::IPixelShader* mini3d::OGL20GraphicsService::GetPixelShader(void)
//{
//	return pCurrentPixelShader;
//}
//void mini3d::OGL20GraphicsService::SetPixelShader(IPixelShader* pPixelShader)
//{
//	
//	if (pCurrentPixelShader == pPixelShader)
//		return;
//
//	PFNGLATTACHSHADERPROC glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
//	PFNGLLINKPROGRAMPROC glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
//
//	if (pPixelShader == 0)
//	{
//		glAttachShader(hProgram, 0);
//		glLinkProgram(hProgram);
//		printLog(hProgram);
//	}
//	else
//	{
//		OGL20PixelShader* pOLG20PixelShader = (OGL20PixelShader*)pPixelShader;
//		glAttachShader(hProgram, pOLG20PixelShader->GetPixelShaderBuffer());
//		glLinkProgram(hProgram);
//		printLog(hProgram);
//	}
//
//	pCurrentPixelShader = pPixelShader;
//}
//
//// Vertex Shader
//mini3d::IVertexShader* mini3d::OGL20GraphicsService::GetVertexShader(void)
//{
//	return pCurrentVertexShader;
//}
//void mini3d::OGL20GraphicsService::SetVertexShader(IVertexShader* pVertexShader)
//{
//
//	PFNGLATTACHSHADERPROC glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
//	PFNGLLINKPROGRAMPROC glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
//
//	if (pCurrentVertexShader == pVertexShader)
//		return;
//
//	if (pVertexShader == 0)
//	{
//		glAttachShader(hProgram, 0);
//		glLinkProgram(hProgram);
//		printLog(hProgram);
//	}
//	else
//	{
//		OGL20VertexShader* pOLG20VertexShader = (OGL20VertexShader*)pVertexShader;
//		glAttachShader(hProgram, pOLG20VertexShader->GetVertexShaderBuffer());
//		glLinkProgram(hProgram);
//		printLog(hProgram);
//	}
//	
//	pCurrentVertexShader = pVertexShader;
//
//}

// Texture
mini3d::ITexture* mini3d::OGL20GraphicsService::GetTexture(unsigned int index)
{
	if (index > GetMaxTextures())
	{
		// Throw error in debug mode;
		return 0;
	}

//	return currentITextures[index];
	return 0;
}
void mini3d::OGL20GraphicsService::SetTexture(ITexture* pTexture, unsigned int index)
{	

	if (index >  GetMaxTextures())
	{
		// TODO: Throw error in debug mode;
		return;
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
		case ITexture::TILE:

			break;
		case ITexture::MIRROR:

			break;
		case ITexture::CLAMP:

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
mini3d::IRenderTarget* mini3d::OGL20GraphicsService::GetRenderTarget(void)
{
	return pCurrentRenderTarget;
}
void mini3d::OGL20GraphicsService::SetRenderTarget(IRenderTarget* pRenderTarget)
{

	if (pRenderTarget == pCurrentRenderTarget)
		return;

	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

	// this cast is "unfailable" (not exception caught). Whoever inherits from IRenderTarget must also inherit from IOGL20RenderTarget
	IOGL20RenderTarget* pOGL20RenderTarget = dynamic_cast<IOGL20RenderTarget*>(pRenderTarget);

	if (pOGL20RenderTarget->GetType() == 1) // if rendertargettexture
	{
		// this cast in "unfailable" becase we tested it in the if-statement we are in
		OGL20RenderTargetTexture* pOGL20RenderTargetTexture = dynamic_cast<OGL20RenderTargetTexture*>(pRenderTarget);

		if (pRenderTarget == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pOGL20RenderTargetTexture->GetRenderTargetBuffer());
		}
	}
	else // screenrendertarget
	{
		// this cast in "unfailable" becase we tested it in the if-statement we are in
		OGL20ScreenRenderTarget* pOGL20ScreenRenderTarget = dynamic_cast<OGL20ScreenRenderTarget*>(pRenderTarget);

		if (pRenderTarget == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			wglMakeCurrent(pOGL20ScreenRenderTarget->GetRenderTargetBuffer(), hRenderContext);
		}
	}

	pCurrentRenderTarget = pRenderTarget;

}

// DepthStencil
mini3d::IDepthStencil* mini3d::OGL20GraphicsService::GetDepthStencil(void)
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
mini3d::IIndexBuffer* mini3d::OGL20GraphicsService::GetIndexBuffer(void)
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
mini3d::IVertexBuffer* mini3d::OGL20GraphicsService::GetVertexBuffer(void)
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
void mini3d::OGL20GraphicsService::SetShaderParameterMatrix(unsigned int index, const float* pData, unsigned int rows, unsigned int columns)
{
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	if (rows == 4 && columns == 4)
		glUniformMatrix4fv(index, 1, GL_FALSE, pData);
}
void mini3d::OGL20GraphicsService::SetShaderParameterFloat(unsigned int index, const float* pData, unsigned int count)
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
void mini3d::OGL20GraphicsService::SetShaderParameterInt(unsigned int index, const int* pData, unsigned int count)
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
void mini3d::OGL20GraphicsService::SetShaderParameterBool(unsigned int index, const bool* pData, unsigned int count)
{
	// not supported
}


// Drawing
void mini3d::OGL20GraphicsService::Draw(void)
{
	BeginScene();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glDrawElements(GL_TRIANGLES, pCurrentIndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
}
void mini3d::OGL20GraphicsService::DrawIndices(unsigned int startIndex, unsigned int numIndices)
{
	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");

	BeginScene();
	glDrawRangeElements(GL_TRIANGLES, startIndex, numIndices + startIndex, numIndices / 3, GL_UNSIGNED_INT, 0);
}

// Clear
void mini3d::OGL20GraphicsService::Clear(int color)
{
	glClearColor((char)color / 256.0f, (char)(color>>2) / 256.0f, (char)(color>>4) / 256.0f, (char)(color>>6) / 256.0f);
	//glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Create Resources
mini3d::IScreenRenderTarget* mini3d::OGL20GraphicsService::CreateScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, IScreenRenderTarget::Quality quality)
{
	return new OGL20ScreenRenderTarget(this, width, height, hWindow, depthTestEnabled, quality);
}
mini3d::IRenderTargetTexture* mini3d::OGL20GraphicsService::CreateRenderTargetTexture(unsigned int width, unsigned int height, bool depthTestEnabled)
{
	return new OGL20RenderTargetTexture(this, width, height, depthTestEnabled);
}
mini3d::IBitmapTexture* mini3d::OGL20GraphicsService::CreateBitmapTexture(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth, ITexture::WrapStyle wrapStyle)
{
	return new OGL20BitmapTexture(this, pBitmap, width, height, bitDepth, wrapStyle);
}
mini3d::IVertexBuffer* mini3d::OGL20GraphicsService::CreateVertexBuffer(void* vertices, unsigned int count, const IVertexBuffer::VertexDeclarationVector& vertexDeclaration)
{
	return new OGL20VertexBuffer(this, vertices, count, vertexDeclaration);
}
mini3d::IIndexBuffer* mini3d::OGL20GraphicsService::CreateIndexBuffer(int* indices, unsigned int count, const IIndexBuffer::CullMode cullMode)
{
	return new OGL20IndexBuffer(this, indices, count);
}
mini3d::IPixelShader* mini3d::OGL20GraphicsService::CreatePixelShader(const IPixelShader::ShaderBytes& shaderBytes)
{
	return new OGL20PixelShader(this, shaderBytes);
}
mini3d::IVertexShader* mini3d::OGL20GraphicsService::CreateVertexShader(const IVertexShader::ShaderBytes& shaderBytes, const IVertexShader::VertexDeclarationVector& vertexDeclaration)
{
	return new OGL20VertexShader(this, shaderBytes, vertexDeclaration);
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
