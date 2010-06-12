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
#define NOMINMAX

#include "mini3d/utilities/Math3d.h"
#include "mini3d/utilities/Window.h"
#include "mini3d/utilities/BinaryFileReader.h"

#include "mini3d/IGraphicsService.h"
#include "mini3d/DirectX9/DX9GraphicsService.h"
//#include "mini3d/OpenGL20/OGL20GraphicsService.h"

#include <math.h>

using namespace mini3d::utilites;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Vertex Data Structure
struct VertexPCT { float x,y,z,w;  float r,g,b,a;  float u,v; };

// Vertex array
VertexPCT vertices[] = {{-5.0f,-5.0f,  5.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f},    { 5.0f,-5.0f,  5.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f},    { 5.0f, 5.0f,  5.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 1.0f},    {-5.0f, 5.0f,  5.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f},
						{-5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f},    { 5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f},    { 5.0f, 5.0f, -5.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 1.0f},    {-5.0f, 5.0f, -5.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f}};

// Index array
unsigned int indices[] = {0, 1, 2, 0, 2, 3,    4, 0, 3, 4, 3, 7,    5, 4, 7, 5, 7, 6,    1, 5, 6, 1, 6, 2,  1, 0, 4, 1, 4, 5,    3, 2, 6, 3, 6, 7};

// create a texture
unsigned int texture[] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFF0000,    0xFFFFFFFF, 0xFF0000FF, 0xFFFF0000, 0xFF00FF00,    0xFF0000FF, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF,    0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF};

// state variables
unsigned int width =  640;
unsigned int height = 480;

int fullscreenWidth =  1680;
int fullscreenHeight = 1050;

int mouseX, mouseY;
float rotX, rotY;
bool fullscreen = false;

// Vertex declaration for the vertex shader
mini3d::IVertexShader::VertexDataType vertexDeclaration[] = { mini3d::IVertexShader::POSITION_FLOAT4, mini3d::IVertexShader::COLOR_FLOAT4, mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2 };

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	// create a window
	mini3d::utilites::Window window(WndProc, width, height);
	HWND hWindow = (HWND)window.GetWindowHandle();
	ShowWindow(hWindow, SW_SHOW);

	// create a graphics service
	mini3d::IGraphicsService* graphics = new mini3d::DX9GraphicsService(false);
	
	// create a render target (mini3d does not have a default render target)
	mini3d::IScreenRenderTarget* pScreenRenderTarget = graphics->CreateScreenRenderTarget(width,height,(int)hWindow, true, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);
	mini3d::IFullscreenRenderTarget* pFullScreenRenderTarget = graphics->CreateFullscreenRenderTarget(fullscreenWidth, fullscreenHeight,(int)hWindow, true, mini3d::IFullscreenRenderTarget::QUALITY_MINIMUM);

	// Create a render target texture
	mini3d::IRenderTargetTexture* pRenderTargetTexture = graphics->CreateRenderTargetTexture(512, 512, true);

	// create index buffer
	mini3d::IIndexBuffer* iBuffer = graphics->CreateIndexBuffer(indices, 36);

	// create vertex buffer
	mini3d::IVertexBuffer* vBuffer = graphics->CreateVertexBuffer(vertices, 8, sizeof(VertexPCT));
	
	// create texture
	mini3d::IBitmapTexture* pTexture = graphics->CreateBitmapTexture(texture, 4, 4);
	
	unsigned int sizeInBytes;
	
	// create vertex shader
	char* shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"vertexshader.hlsl", sizeInBytes);
	mini3d::IVertexShader* pVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);
	delete shaderBytes;

	// create pixel shader
	char* shaderBytes2 = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"pixelshader.hlsl", sizeInBytes);
	mini3d::IPixelShader* pPixelShader = graphics->CreatePixelShader(shaderBytes2, sizeInBytes);
	delete shaderBytes2;

	// create shader program
	mini3d::IShaderProgram* pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);

	// set render prarameters
	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetShaderProgram(pShaderProgram);
	
	
	// run the application while the window is showing
	while(true)
	{
		// For keeping track of window messages
		MSG Msg;

		// Matrix4f
		Matrix4 viewMatrixE;
		Matrix4 projectionMatrixE;

		// keeps track of what texture unit the texture is assigned to. This value is sent to the shader program
		int* textureUnit = new int(0);

		// keeps track of the current frame, for animation
		double frame = 0;

		// loop while the window is not closed
		while(GetMessage(&Msg, NULL, 0, 0) > 0)
		{

			// update camera
			mini3d::utilites::math3d::setuplookat(&viewMatrixE._00, (float*)&Vector3(30 * cos(rotY) * cos((float)rotX), 30 * sin(rotY), 30 * cos(rotY) * sin(rotX)), (float*)&Vector3(0,0,0), (float*)&Vector3(0,-1,0));
			mini3d::utilites::math3d::BuildPerspProjMat(&projectionMatrixE._00, (float)(3.1415f / 4.0f), (float)width/(float)height, 40.0f / 100.0f, 80.0f);
			Matrix4 viewProjectionMatrixE = viewMatrixE * projectionMatrixE;

			// set up the scene with the renderTargetTexture
			graphics->SetRenderTarget(pRenderTargetTexture);
			graphics->SetTexture(pTexture, 0);

			// set a shader parameter
			graphics->SetShaderParameterMatrix(0, &viewProjectionMatrixE._00, 4, 4);
			graphics->SetShaderParameterInt(1, textureUnit, 1);
			
			// clear render target with color
			graphics->Clear(0x5555FFFF);

			// draw the scene to the renderTargetTexture
			graphics->Draw();
			
			if (fullscreen == true)
				graphics->SetRenderTarget(pFullScreenRenderTarget);
			else
				graphics->SetRenderTarget(pScreenRenderTarget);
			
			graphics->SetTexture(pRenderTargetTexture, 0);

			//// clear render target with color
			graphics->Clear(0xFFFFFFFF);

			//// Draw the scene to the screenRenderTarget
			graphics->Draw();

			// do a flip
			if (fullscreen == true)
				pFullScreenRenderTarget->Display();
			else
				pScreenRenderTarget->Display();
			

			// windows message stuff
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			// update frame
			frame += 0.01;
		}

		delete vBuffer;
		delete iBuffer;
		delete pTexture;
		delete pPixelShader;
		delete pVertexShader;
		delete pScreenRenderTarget;
		delete pRenderTargetTexture;
		delete graphics;

		return Msg.wParam;
	}

	return 1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_SIZE:
			width = LOWORD(lParam);
			height = HIWORD(lParam);
		break;
		case WM_LBUTTONDOWN:
			mouseX = LOWORD(lParam); 
			mouseY = HIWORD(lParam);
			SetCapture(hwnd);
		break;
		case WM_KEYDOWN:
			if ((wParam & VK_F12) == VK_F12)
			{
				fullscreen = !fullscreen;
			}
		break;
			
		break;
		case WM_MOUSEMOVE:
			if ((wParam & MK_LBUTTON) == MK_LBUTTON)
			{
				int x = LOWORD(lParam); 
				int y = HIWORD(lParam);

				rotX += (x - mouseX) / 100.0f;
				rotY -= (y - mouseY) / 100.0f;

				if (rotY > 3.1416f / 2.0f)
					rotY = 3.1416f / 2.0f;
				else if (rotY < -3.1416f / 2.0f)
					rotY = -3.1416f / 2.0f;

				mouseX = x;
				mouseY = y;
			}
		break;
		case WM_LBUTTONUP:
		{
			ReleaseCapture();
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
