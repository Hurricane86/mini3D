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


#include <math.h>
#include "..\..\mini3d.h"
#include <Windows.h>

// ----- FORWARD DECLARATIONS -------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateViewProjectionMatrix();
void UpdateWindowViewProjectionMatrix();
mini3d::IVertexShader* LoadVertexShaderFromFile(WCHAR* fileName);
mini3d::IPixelShader* LoadPixelShaderFromFile(WCHAR* fileName);

// ----- GEOMETRY DATA --------------------------------------------------------

// Vertex Data Structure
struct VertexPCT { float x,y,z,w;  float r,g,b,a;  float u,v; };

// Vertex array
VertexPCT vertices[] = {
	// Front
	{-1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.0f, 0.0f}, // Corner 0
	{ 1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.0f, 0.0f}, // Corner 1
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.0f, 0.0f}, // Corner 2
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.0f, 0.0f}, // Corner 3
						
	// Back
	{ 1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.0f, 0.0f}, // Corner 4
	{-1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.0f, 0.0f}, // Corner 5
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.0f, 0.0f}, // Corner 6
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.0f, 0.0f}, // Corner 7
};

VertexPCT windowVertices[] = {
	// Glass
	{-2.0f, -2.0f,  3.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,   0.05f,  0.05f}, // Corner 0
	{ 2.0f, -2.0f,  3.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  -0.05f,  0.05f}, // Corner 1
	{ 2.0f,  2.0f,  3.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  -0.05f, -0.05f}, // Corner 2
	{-2.0f,  2.0f,  3.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,   0.05f, -0.05f}, // Corner 3
};


// Index array
unsigned int indices[] = {0, 1, 2, 0, 2, 3,    4, 5, 6, 4, 6, 7,    1, 4, 7, 1, 7, 2,    5, 0, 3, 5, 3, 6,    5, 4, 1, 5, 1, 0,    7, 6, 3, 7, 3, 2};

unsigned int windowIndices[] = {0, 1, 2, 0, 2, 3};

// Vertex declaration for the vertex shader
mini3d::IVertexShader::VertexDataType vertexDeclaration[] = { mini3d::IVertexShader::POSITION_FLOAT4, mini3d::IVertexShader::COLOR_FLOAT4, mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2 };


// ----- DECLARE VARIABLES ----------------------------------------------------

// View and projection matrices for setting up camera view
mini3d::utilites::Matrix4 viewMatrixE;
mini3d::utilites::Matrix4 projectionMatrixE;

// state variables
bool fullscreen = false;

// Keeps track of mouse movement
int mouseX, mouseY;

// Keeps track of the view angle and camera distance
float rotX = 3.1415f / 4.0f; // three quater spin
float rotY = -3.1415f / 4.0f; // three quater spin
float distance = 10.0f; // medium distance :)

// current window size
int width = 640;
int height = 480;

// For keeping track of window messages
MSG Msg;


// ----- DECLARE GRAPHICS RESOURCES -------------------------------------------

// Graphics Service
mini3d::IGraphicsService* graphics;

// Graphics Resources
mini3d::IWindowRenderTarget* pScreenRenderTarget;
mini3d::IFullscreenRenderTarget* pFullScreenRenderTarget;

mini3d::IRenderTargetTexture* pWindowRenderTargetTexture;

mini3d::IIndexBuffer* iBuffer;
mini3d::IVertexBuffer* vBuffer;

mini3d::IIndexBuffer* pWindowIndexBuffer;
mini3d::IVertexBuffer* pWindowVertexBuffer;

mini3d::IVertexShader* pVertexShader;
mini3d::IPixelShader* pPixelShader;
mini3d::IShaderProgram* pShaderProgram;

mini3d::IVertexShader* pWindowVertexShader;
mini3d::IPixelShader* pWindowPixelShader;
mini3d::IShaderProgram* pWindowShaderProgram;


// Tutorial Application
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	
	// ----- CREATE A WINDOW --------------------------------------------------

	// create a window
	mini3d::utilites::Window window(WndProc, 640, 480);
	HWND hWindow = (HWND)window.GetWindowHandle();
	ShowWindow(hWindow, SW_SHOW);


	// ----- CREATE GRAPHICS SERVICE ------------------------------------------
	
	graphics = new mini3d::DX9GraphicsService(false);
	

	// ----- CREATE GRAPHICS RESOURCES ----------------------------------------

	// create a render target (mini3d does not have a default render target)
	pScreenRenderTarget = graphics->CreateWindowRenderTarget(640, 480,(int)hWindow, true, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);
	pFullScreenRenderTarget = graphics->CreateFullscreenRenderTarget(1680, 1050,(int)hWindow, true, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);

	pWindowRenderTargetTexture = graphics->CreateRenderTargetTexture(640, 480, true);

	// create index and vertex buffer
	iBuffer = graphics->CreateIndexBuffer(indices, 36);
	vBuffer = graphics->CreateVertexBuffer(vertices, 8, sizeof(VertexPCT));

	// create index and vertex buffer
	pWindowIndexBuffer = graphics->CreateIndexBuffer(windowIndices, 6);
	pWindowVertexBuffer = graphics->CreateVertexBuffer(windowVertices, 4, sizeof(VertexPCT));

	// create scene shaders
	pVertexShader = LoadVertexShaderFromFile(L"shaders/hlsl/vertexshader.hlsl");
	pPixelShader = LoadPixelShaderFromFile(L"shaders/hlsl/pixelshader.hlsl");
	pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);

	pWindowVertexShader = LoadVertexShaderFromFile(L"shaders/hlsl/windowvertexshader.hlsl");
	pWindowPixelShader = LoadPixelShaderFromFile(L"shaders/hlsl/windowpixelshader.hlsl");
	pWindowShaderProgram = graphics->CreateShaderProgram(pWindowVertexShader, pWindowPixelShader);


	// ----- CONFIIGURE GRAPHICS PIPELINE -------------------------------------

	// Set the ViewProjection matrix
	UpdateViewProjectionMatrix();
	UpdateWindowViewProjectionMatrix();

	// ----- RENDER LOOP ------------------------------------------------------

	// loop while the window is not closed
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		graphics->SetTexture(0,0);
		graphics->SetRenderTarget(pWindowRenderTargetTexture);
		graphics->SetShaderProgram(pShaderProgram);
		graphics->SetIndexBuffer(iBuffer);
		graphics->SetVertexBuffer(vBuffer);
		
		// clear render target with color
		graphics->Clear(0xFF888888);

		graphics->Draw();


		// set the correct rendertarget depeding on fullscreen mode
		if (fullscreen == true)
			graphics->SetRenderTarget(pFullScreenRenderTarget);
		else
			graphics->SetRenderTarget(pScreenRenderTarget);

		// clear render target with color
		graphics->Clear(0xFF888888);

		// draw the scene to the renderTargetTexture
		graphics->Draw();


		// Set the Texture
		graphics->SetShaderProgram(pWindowShaderProgram);
		graphics->SetTexture(pWindowRenderTargetTexture, 0);

		graphics->SetIndexBuffer(pWindowIndexBuffer);
		graphics->SetVertexBuffer(pWindowVertexBuffer);
		
		graphics->Draw();

		// do a flip
		// set the correct rendertarget depeding on fullscreen mode
		if (fullscreen == true)
			pFullScreenRenderTarget->Display();
		else
			pScreenRenderTarget->Display();

		// window message stuff
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}


	// ----- DELETE RESOURCES ----------------------------------------------------

	delete vBuffer;
	delete iBuffer;
	delete pWindowIndexBuffer;
	delete pWindowVertexBuffer;
	delete pPixelShader;
	delete pVertexShader;
	delete pShaderProgram;
	delete pScreenRenderTarget;
	delete pFullScreenRenderTarget;
	delete pWindowRenderTargetTexture;
	delete graphics;

	return Msg.wParam;
}

// sets the view, projection matrix as a shader parameter
void UpdateViewProjectionMatrix()
{
	mini3d::utilites::Vector3 eye(distance * cos(rotY) * cos((float)rotX), distance * sin(rotY), distance * cos(rotY) * sin(rotX));
	mini3d::utilites::Vector3 target(0,0,0);
	mini3d::utilites::Vector3 up(0,-1,0);

	// update camera
	mini3d::utilites::math3d::setuplookat(&viewMatrixE._00, (float*)&eye, (float*)&target, (float*)&up);
	mini3d::utilites::math3d::BuildPerspProjMat(&projectionMatrixE._00, (float)(3.1415f / 4.0f), (float)width/(float)height, 40.0f / 100.0f, 80.0f);
	mini3d::utilites::Matrix4 viewProjectionMatrixE = viewMatrixE * projectionMatrixE;

	// set a shader parameter
	graphics->SetShaderParameterMatrix(0, &viewProjectionMatrixE._00, 4, 4);
}

// sets the view, projection matrix as a shader parameter
void UpdateWindowViewProjectionMatrix()
{
	mini3d::utilites::Vector3 eye(distance * cos(rotY) * cos((float)rotX), distance * sin(rotY), distance * cos(rotY) * sin(rotX));
	mini3d::utilites::Vector3 target(0,0,0);
	mini3d::utilites::Vector3 up(0,-1,0);

	// update camera
	mini3d::utilites::math3d::setuplookat(&viewMatrixE._00, (float*)&eye, (float*)&target, (float*)&up);
	mini3d::utilites::math3d::BuildPerspProjMat(&projectionMatrixE._00, (float)(3.1415f / 4.0f), (float)width/(float)height, 40.0f / 100.0f, 80.0f);
	mini3d::utilites::Matrix4 viewProjectionMatrixE = viewMatrixE * projectionMatrixE;

	// set a shader parameter
	graphics->SetShaderParameterMatrix(4, &viewProjectionMatrixE._00, 4, 4);
	graphics->SetShaderParameterFloat(8, (float*)&eye, 3);
}

mini3d::IVertexShader* LoadVertexShaderFromFile(WCHAR* fileName)
{
	// create vertex shader
	unsigned int sizeInBytes;
	
	char* shaderBytes;
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(fileName, sizeInBytes);
	mini3d::IVertexShader* pVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);

	delete shaderBytes;
	return pVertexShader;
}

mini3d::IPixelShader* LoadPixelShaderFromFile(WCHAR* fileName)
{
	// create Pixel shader
	unsigned int sizeInBytes;

	char* shaderBytes;
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(fileName, sizeInBytes);

	mini3d::IPixelShader* pPixelShader = graphics->CreatePixelShader(shaderBytes, sizeInBytes);

	delete shaderBytes;
	return pPixelShader;
}

// callback for our window, handles window messages
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
			UpdateViewProjectionMatrix();
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
		case WM_MOUSEMOVE:
			if ((wParam & MK_LBUTTON) == MK_LBUTTON)
			{
				int x = LOWORD(lParam); 
				int y = HIWORD(lParam);

				rotX += (x - mouseX) / 100.0f;
				rotY -= (y - mouseY) / 100.0f;

				if (rotY > 3.1415f / 2.0f)
					rotY = 3.1415f / 2.0f;
				else if (rotY < -3.1416f / 2.0f)
					rotY = -3.1415f / 2.0f;
								
				mouseX = x;
				mouseY = y;
				UpdateViewProjectionMatrix();
				UpdateWindowViewProjectionMatrix();
			}
		break;
		case WM_LBUTTONUP:
			ReleaseCapture();
		break;
		case WM_MOUSEWHEEL:
			{
				int zWheel = GET_WHEEL_DELTA_WPARAM(wParam);
				float zWheelf = (float)zWheel;
				distance -=  (float)zWheel * 0.01f;
				UpdateViewProjectionMatrix();
				UpdateWindowViewProjectionMatrix();
			}
		break;
		default:
			// return the window message to the default window process
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

