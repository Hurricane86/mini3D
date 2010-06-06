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

#include "mini3d/utilities/math3d.h"
#include "mini3d/utilities/BinaryFileReader.h"

#include <Eigen/Core>
#include <eigen/src/Core/Matrix.h>
#include <Eigen/Geometry>

#include "d3d9.h"
#include <d3dx9.h>

#include "mini3d/IGraphicsService.h"
#include "mini3d/DirectX9/DX9GraphicsService.h"
//#include "mini3d/OpenGL20/OGL20GraphicsService.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;
USING_PART_OF_NAMESPACE_EIGEN
#define Vector3 D3DXVECTOR3

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
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

HWND createWindow(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"mini3d";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


	HWND hWindow = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"mini3d",
        L"The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 512, 512,
        NULL, NULL, hInstance, NULL);

	return hWindow;
}

// Vertex Data Structure
struct VertexPCT { float x,y,z,w;  float r,g,b,a;  float u,v; };

// Vertex array
VertexPCT vertices[] = {{-5.0f,-5.0f, 5.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f},
						{ 5.0f,-5.0f, 5.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.99f, 0.0f},
						{ 5.0f, 5.0f, 5.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.99f, 0.99f},
						{-5.0f, 5.0f, 5.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.99f},
						
						{-5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f},
						{ 5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.99f, 0.0f},
						{ 5.0f, 5.0f, -5.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.99f, 0.99f},
						{-5.0f, 5.0f, -5.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.99f}};

// Index array
int indices[] = {0, 1, 2, 0, 2, 3,   4, 0, 3, 4, 3, 7,  5, 4, 7, 5, 7, 6,  1, 5, 6, 1, 6, 2,  1, 0, 4, 1, 4, 5,  3, 2, 6, 3, 6, 7};

// Vertex declaration for the vertex shader
mini3d::IVertexShader::VertexDataType vertexDeclaration[] = { mini3d::IVertexShader::POSITION_FLOAT4, mini3d::IVertexShader::COLOR_FLOAT4, mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2 };

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	// create a window
	HWND hWindow = createWindow();
		ShowWindow(hWindow, SW_SHOW);
		UpdateWindow(hWindow);

	// create a graphics service
	mini3d::IGraphicsService* graphics = new mini3d::DX9GraphicsService(false);
	
	// create a render target (mini3d does not have a default render target)
	mini3d::IScreenRenderTarget* pScreenRenderTarget = graphics->CreateScreenRenderTarget(512,512,(int)hWindow, true, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);
	//mini3d::IFullscreenRenderTarget* pScreenRenderTarget = graphics->CreateFullscreenRenderTarget(1280,800,(int)hWindow, false, mini3d::IFullscreenRenderTarget::QUALITY_MINIMUM);

	// Create a render target texture
	mini3d::IRenderTargetTexture* pRenderTargetTexture = graphics->CreateRenderTargetTexture(512, 512, true);

	// create index buffer
	mini3d::IIndexBuffer* iBuffer = graphics->CreateIndexBuffer(indices, 36, mini3d::IIndexBuffer::INT_32, mini3d::IIndexBuffer::CULL_NONE);

	// create vertex buffer
	mini3d::IVertexBuffer* vBuffer = graphics->CreateVertexBuffer(vertices, 8, sizeof(VertexPCT));
	
	// create texture
	int* pBitmap = new int[512 * 512 * 4]; for(int i = 0; i < 512 * 512; i++) { pBitmap[i] = 0xFFFFFFFF * ((i / 128) % 2); }
	mini3d::IBitmapTexture* pTexture = graphics->CreateBitmapTexture(pBitmap, 512, 512);
	delete[] pBitmap;
	
	unsigned int sizeInBytes;
	
	// create vertex shader
	char* shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"vertexshader.hlsl", sizeInBytes);
	mini3d::IVertexShader* pVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);

	// create pixel shader
	char* shaderBytes2 = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"pixelshader.hlsl", sizeInBytes);
	mini3d::IPixelShader* pPixelShader = graphics->CreatePixelShader(shaderBytes2, sizeInBytes);

	// create shader program
	mini3d::IShaderProgram* pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);

	// set render prarameters
	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetShaderProgram(pShaderProgram);
	
	
	// run the application while the window is shwoing
	while(true)
	{
		// For keeping track of window messages
		MSG Msg;

		// Matrix4f
		Matrix4f viewMatrixE;
		Matrix4f projectionMatrixE;

		// keeps track of what texture unit the texture is assigned to. This value is sent to the shader program
		int* textureUnit = new int(0);

		// keeps track of the current frame, for animation
		double frame = 0;

		// loop while the window is not closed
		while(GetMessage(&Msg, NULL, 0, 0) > 0)
		{

			// update camera
			mini3d::utilites::math3d::setuplookat(&viewMatrixE[0], &Vector3f(20 * cos(frame), -8, 20 * sin(frame)).x(), &Vector3f(0,0,0).x(), &Vector3f(0,-1,0).x());
			mini3d::utilites::math3d::BuildPerspProjMat(&projectionMatrixE[0], (float)(3.1415f / 4.0f), 1, 40.0f / 100.0f, 80.0f);
			Matrix4f viewProjectionMatrixE = projectionMatrixE * viewMatrixE;

			// set up the scene with the renderTargetTexture
			graphics->SetRenderTarget(pRenderTargetTexture);
			graphics->SetTexture(pTexture, 0);

			// set a shader parameter
			graphics->SetShaderParameterMatrix(0, &viewProjectionMatrixE[0], 4, 4);
			graphics->SetShaderParameterInt(1, textureUnit, 1);
			
			// clear render target with color
			graphics->Clear(0x55FF55FF);

			// draw the scene to the renderTargetTexture
			graphics->Draw();
			
			graphics->SetRenderTarget(pScreenRenderTarget);
			graphics->SetTexture(pRenderTargetTexture, 0);

			//// clear render target with color
			graphics->Clear(0xFFFFFFFF);

			//// Draw the scene to the screenRenderTarget
			graphics->Draw();

			// do a flip
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
		delete graphics;

		return Msg.wParam;
	}



	return 1;
}
