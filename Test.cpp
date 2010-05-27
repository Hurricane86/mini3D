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

#include "d3d9.h"
#include <d3dx9.h>

#include "mini3D/IGraphicsService.h"
#include "mini3D/DirectX9/DX9GraphicsService.h"
#include "mini3D/OpenGL20/OGL20GraphicsService.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

#define Vector3 D3DXVECTOR3

struct VertexPCT
{
	float x,y,z,w;
	float r,g,b,a;
	float u,v;
};

VertexPCT vertices[] = {{-5.0f,-5.0f, 5.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f},
						{ 5.0f,-5.0f, 5.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.99f, 0.0f},
						{ 5.0f, 5.0f, 5.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.99f, 0.99f},
						{-5.0f, 5.0f, 5.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.99f},
						
						{-5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f},
						{ 5.0f,-5.0f, -5.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.99f, 0.0f},
						{ 5.0f, 5.0f, -5.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.99f, 0.99f},
						{-5.0f, 5.0f, -5.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.99f}};

int indices[] = {0, 1, 2, 0, 2, 3,   4, 0, 3, 4, 3, 7,  5, 4, 7, 5, 7, 6,  1, 5, 6, 1, 6, 2,  1, 0, 4, 1, 4, 5,  3, 2, 6, 3, 6, 7};

mini3d::IPixelShader::ShaderBytes ShaderBytesFromFile(wstring file)
{
	
	mini3d::IPixelShader::ShaderBytes shaderBytes;

	fstream fileStream(file.c_str(), ios_base::binary | ios_base::in);
	if (fileStream)
	{
		fileStream.seekg(0, ios::end);
		long fileSize = long(fileStream.tellg()); // pretty safe downcast since shader files will not be gazillions of bytes long
		fileStream.seekg(0, ios::beg);

		shaderBytes.resize(fileSize);
		fileStream.read(&shaderBytes[0], fileSize);
		fileStream.close();
	}
	return shaderBytes;
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
    wc.lpszClassName = L"OpenGL";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


	HWND hWindow = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"OpenGL",
        L"The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 512, 512,
        NULL, NULL, hInstance, NULL);

	return hWindow;
}

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	// create a window
	HWND hWindow = createWindow();
		ShowWindow(hWindow, SW_SHOW);
		UpdateWindow(hWindow);

	// create a vertex declaration for the vertex buffer and the vertex shader
	mini3d::IVertexShader::VertexDeclarationVector vd;
		vd.push_back(mini3d::IVertexShader::POSITION_FLOAT4);
		vd.push_back(mini3d::IVertexShader::COLOR_FLOAT4);
		vd.push_back(mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2);

	// create a graphics service
	mini3d::IGraphicsService* graphics = new mini3d::DX9GraphicsService(false);
	
	// create a render target (mini3d does not have a default render target)
	mini3d::IScreenRenderTarget* pScreenRenderTarget = graphics->CreateScreenRenderTarget(512,512,(int)hWindow, false, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);
	//mini3d::IFullscreenRenderTarget* pScreenRenderTarget = graphics->CreateFullscreenRenderTarget(1280,800,(int)hWindow, false, mini3d::IFullscreenRenderTarget::QUALITY_MINIMUM);

	mini3d::IRenderTargetTexture* pRenderTargetTexture = graphics->CreateRenderTargetTexture(512, 512, true);
	

	// create index buffer
	int* pIndices = new int[36];
	memcpy(pIndices, &indices, sizeof(indices));
	mini3d::IIndexBuffer* iBuffer = graphics->CreateIndexBuffer(pIndices, 36, mini3d::IIndexBuffer::CULL_NONE);

	// create vertex buffer
	VertexPCT* pVertices = new VertexPCT[8];
	memcpy(pVertices, &vertices, sizeof(vertices));
	mini3d::IVertexBuffer* vBuffer = graphics->CreateVertexBuffer(pVertices, 8, vd);
	
	// create texture
	int* pBitmap = new int[512 * 512 * 4];
	for(int i = 0; i < 512 * 512; i++)
	{
		pBitmap[i] = 0xFFFFFFFF * ((i / 128) % 2);
	}
	mini3d::IBitmapTexture* pTexture = graphics->CreateBitmapTexture(pBitmap, 512, 512);

	// create vertex shader
	mini3d::IVertexShader::ShaderBytes shaderBytes2 = ShaderBytesFromFile(L"vertexshader.hlsl"); 
	mini3d::IVertexShader* pVertexShader = graphics->CreateVertexShader(shaderBytes2, vd);

	// create pixel shader
	mini3d::IPixelShader::ShaderBytes shaderBytes = ShaderBytesFromFile(L"pixelshader.hlsl");
	mini3d::IPixelShader* pPixelShader = graphics->CreatePixelShader(shaderBytes);
	
	// create shader program
	mini3d::IShaderProgram* pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);

	// create a view projection matrix
	// no matrix math in mini3d, so we use the d3dx stuff... could be any other lib here just as well...
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
		
	D3DXMatrixLookAtLH(&viewMatrix, &Vector3(10, -8, 30), &Vector3(0,0,0), &Vector3(0,-1,0));
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)(3.1415f / 4.0f), 1, 40.0f / 1000.0f, 40.0f);

	D3DXMATRIX viewProjectionMatrix = viewMatrix * projectionMatrix;
	D3DXMATRIX viewProjectionMatrixTranspose;
	D3DXMatrixTranspose(&viewProjectionMatrixTranspose, &viewProjectionMatrix);

	// set render prarameters
	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetShaderProgram(pShaderProgram);

	int* textureUnit = new int(0);

	// run the message loop
	MSG Msg;
	
	double frame = 0;

	// run the application while the window is shwoing
	while(true)
	{
	    while(GetMessage(&Msg, NULL, 0, 0) > 0)
		{

			// update camera
			D3DXMatrixLookAtLH(&viewMatrix, &Vector3(20 * cos(frame), -8, 25 * sin(frame)), &Vector3(0,0,0), &Vector3(0,-1,0));
			D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)(3.1415f / 4.0f), 1, 40.0f / 1000.0f, 40.0f);

			D3DXMATRIX viewProjectionMatrix = viewMatrix * projectionMatrix;
			D3DXMATRIX viewProjectionMatrixTranspose;
			D3DXMatrixTranspose(&viewProjectionMatrixTranspose, &viewProjectionMatrix);

			// set a shader parameter
			graphics->SetShaderParameterMatrix(0, &viewProjectionMatrix._11, 4, 4);
			graphics->SetShaderParameterInt(1, textureUnit, 1);
			
			// set up the scene with the renderTargetTexture
			graphics->SetRenderTarget(pRenderTargetTexture);
			graphics->SetTexture(pTexture, 0);

			// clear render target with color
			graphics->Clear(0x55FF55FF);

			// draw the scene to the renderTargetTexture
			graphics->Draw();
			
			graphics->SetRenderTarget(pScreenRenderTarget);
			graphics->SetTexture(pRenderTargetTexture, 0);

			// clear render target with color
			graphics->Clear(0xFFFFFFFF);

			// Draw the scene to the screenRenderTarget
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

