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
void UpdateLightMatrix();

// ----- GEOMETRY DATA --------------------------------------------------------

// Vertex Data Structure
struct VertexPCT { float x,y,z,w;  float r,g,b,a;  float u,v; };

// Vertex array
VertexPCT vertices[] = {
	// Front
	{-1.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.5f, 0.0f}, // Corner 0
	{ 1.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.75f, 0.0f}, // Corner 1
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.75f, 0.5f}, // Corner 2
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.5f, 0.5f}, // Corner 3

	// Back
	{ 1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.5f, 0.5f}, // Corner 4
	{-1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.75f, 0.5f}, // Corner 5
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.75f, 1.0f}, // Corner 6
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.5f, 1.0f}, // Corner 7

	// Right
	{ 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.5f}, // Corner 1
	{ 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.5f, 0.5f}, // Corner 4
	{ 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.5f, 1.0f}, // Corner 7
	{ 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f}, // Corner 2

	// Left
	{-1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.0f}, // Corner 5
	{-1.0f, -1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.5f, 0.0f}, // Corner 0
	{-1.0f,  1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.5f, 0.5f}, // Corner 3
	{-1.0f,  1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.5f}, // Corner 6

	// Top
	{-1.0f, -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f}, // Corner 5
	{ 1.0f, -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.25f, 0.0f}, // Corner 4
	{ 1.0f, -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.25f, 0.5f}, // Corner 1
	{-1.0f, -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.5f}, // Corner 0

	//Bottom
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.5f}, // Corner 7
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.5f}, // Corner 6
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 1.0f}, // Corner 3
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f}  // Corner 2
};

VertexPCT vertices2[] = {
	{-5.0f, 1.0f, -5.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f}, // Corner 0
	{ 5.0f, 1.0f, -5.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.25f, 0.0f}, // Corner 1
	{ 5.0f, 1.0f,  5.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.25f, 0.5f}, // Corner 2
	{-5.0f, 1.0f,  5.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.5f}  // Corner 3
};

// Index array
unsigned int indices[] = {0, 1, 2, 0, 2, 3,    4, 5, 6, 4, 6, 7,    8, 9, 10, 8, 10, 11,    12, 13, 14, 12, 14, 15,    16, 17, 18, 16, 18, 19,    20, 21, 22, 20, 22, 23};

unsigned int indices2[] = {0, 1, 2, 0, 2, 3};

// Vertex declaration for the vertex shader
mini3d::IVertexShader::VertexDataType vertexDeclaration[] = { mini3d::IVertexShader::POSITION_FLOAT4, mini3d::IVertexShader::NORMAL_FLOAT4, mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2 };


// ----- DECLARE VARIABLES ----------------------------------------------------

// state variables
bool fullscreen = false;

// Keeps track of mouse movement
int mouseX, mouseY;

// Keeps track of the view angle and camera distance
float rotX = 3.1415f / 4.0f; // three quater spin
float rotY = -3.1415f / 4.0f; // three quater spin
float distance = 10.0f; // medium distance :)

// Keeps track of the light animation
float lightAnimationProgress = 0.5f;

// current window size
int width = 640;
int height = 480;

// For keeping track of window messages
MSG Msg;


// ----- DECLARE GRAPHICS RESOURCES -------------------------------------------

// Graphics Service
mini3d::IGraphicsService* graphics;

// Graphics Resources
mini3d::IScreenRenderTarget* pScreenRenderTarget;
mini3d::IFullscreenRenderTarget* pFullScreenRenderTarget;

mini3d::IRenderTargetTexture* pShadowRenderTargetTexture;

mini3d::IIndexBuffer* iBuffer;
mini3d::IVertexBuffer* vBuffer;

mini3d::IIndexBuffer* iBufferGround;
mini3d::IVertexBuffer* vBufferGround;

mini3d::IBitmapTexture* pTexture;

mini3d::IVertexShader* pSceneVertexShader;
mini3d::IPixelShader* pScenePixelShader;
mini3d::IShaderProgram* pSceneShaderProgram;

mini3d::IVertexShader* pShadowVertexShader;
mini3d::IPixelShader* pShadowPixelShader;
mini3d::IShaderProgram* pShadowShaderProgram;


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
	pScreenRenderTarget = graphics->CreateScreenRenderTarget(640, 480,(int)hWindow, true, mini3d::IScreenRenderTarget::QUALITY_MINIMUM);
	pFullScreenRenderTarget = graphics->CreateFullscreenRenderTarget(1680, 1050,(int)hWindow, true, mini3d::IFullscreenRenderTarget::QUALITY_MINIMUM);

	// Shadow render target texture
	pShadowRenderTargetTexture = graphics->CreateRenderTargetTexture(512, 512, true);

	// create index buffer
	iBuffer = graphics->CreateIndexBuffer(indices, 36);
	iBufferGround = graphics->CreateIndexBuffer(indices2, 6);

	// create vertex buffer
	vBuffer = graphics->CreateVertexBuffer(vertices, 24, sizeof(VertexPCT));
	vBufferGround = graphics->CreateVertexBuffer(vertices2, 4, sizeof(VertexPCT));

	// create texture
	unsigned int bitmapWidth, bitmapHeight;
	void* pBitmap = mini3d::utilites::PNGReader::LoadPNGFromFile(L"textures/box.png", bitmapWidth, bitmapHeight);
	pTexture = graphics->CreateBitmapTexture(pBitmap, bitmapWidth, bitmapHeight); 
	delete pBitmap;

	unsigned int sizeInBytes;
	
	// create vertex shader
	char* shaderBytes;
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"shaders/hlsl/scenevs.hlsl", sizeInBytes);
	pSceneVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);
	delete shaderBytes;

	// create pixel shader
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"shaders/hlsl/sceneps.hlsl", sizeInBytes);
	pScenePixelShader = graphics->CreatePixelShader(shaderBytes, sizeInBytes);
	delete shaderBytes;

	// create shadow vertex shader
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"shaders/hlsl/shadowvs.hlsl", sizeInBytes);
	pShadowVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);
	delete shaderBytes;

	// create shadow pixel shader
	shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile(L"shaders/hlsl/shadowps.hlsl", sizeInBytes);
	pShadowPixelShader = graphics->CreatePixelShader(shaderBytes, sizeInBytes);
	delete shaderBytes;

	// ----- VARIABLES FOR RENDER LOOP ----------------------------------------
	
	// keeps track of what texture unit the texture is assigned to. This value is sent to the shader program
	int* pTextureUnit = new int(0);
	int* pTextureUnit2 = new int(1);

	// create shader program
	pSceneShaderProgram = graphics->CreateShaderProgram(pSceneVertexShader, pScenePixelShader);
	pShadowShaderProgram = graphics->CreateShaderProgram(pShadowVertexShader, pShadowPixelShader);


	// ----- CONFIIGURE GRAPHICS PIPELINE -------------------------------------

	// set render prarameters
	//graphics->SetIndexBuffer(iBufferGround);
	//graphics->SetVertexBuffer(vBufferGround);
	graphics->SetShaderProgram(pShadowShaderProgram);
	graphics->SetRenderTarget(pScreenRenderTarget);

	// Set the Texture
	graphics->SetTexture(pTexture, 0);

	// Set the ViewProjection matrix
	UpdateViewProjectionMatrix();

	// set the viewprojection parameter for the shader
	UpdateLightMatrix();



	// ----- RENDER LOOP ------------------------------------------------------

	// loop while the window is not closed
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		// clear the render target texture (can not be mounted as both texture and render target)
		graphics->SetTexture(0, 1);

		// set our shadow as the render target texture
		graphics->SetRenderTarget(pShadowRenderTargetTexture);

		// Set the shadow shader program
		graphics->SetShaderProgram(pShadowShaderProgram);

		// set the usual texture as texture
		graphics->SetTexture(pTexture, 0);

		// clear render target with color
		graphics->Clear(0xFFFFFFFF);

		// Set geometry
		graphics->SetIndexBuffer(iBuffer);
		graphics->SetVertexBuffer(vBuffer);

		// draw the scene to the renderTargetTexture
		graphics->Draw();

		// set the correct rendertarget depeding on fullscreen mode
		if (fullscreen == true)
			graphics->SetRenderTarget(pFullScreenRenderTarget);
		else
			graphics->SetRenderTarget(pScreenRenderTarget);

		// set the scene shader program
		graphics->SetShaderProgram(pSceneShaderProgram);

		// set the shadow texture as texture
		graphics->SetTexture(pShadowRenderTargetTexture, 1);
		graphics->SetTexture(pTexture, 0);

		// set the view projection parameter for the shader
		UpdateViewProjectionMatrix();

		// clear render target with color
		graphics->Clear(0xFF606580);

		// Set geometry
		graphics->SetIndexBuffer(iBuffer);
		graphics->SetVertexBuffer(vBuffer);

		// draw the scene to the renderTargetTexture
		graphics->Draw();

		// Set geometry
		graphics->SetIndexBuffer(iBufferGround);
		graphics->SetVertexBuffer(vBufferGround);

		// draw the scene to the renderTargetTexture
		graphics->Draw();


		// do a flip
		if (fullscreen == true)
			pFullScreenRenderTarget->Display();
		else
			pScreenRenderTarget->Display();

		// window message stuff
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);

		lightAnimationProgress += 0.01f;

		// set the viewprojection parameter for the shader
		UpdateLightMatrix();

	}


	// ----- DELETE RESOURCES ----------------------------------------------------

	delete vBuffer;
	delete iBuffer;
	delete pTexture;

	delete pScenePixelShader;
	delete pSceneVertexShader;
	delete pSceneShaderProgram;

	delete pShadowPixelShader;
	delete pShadowVertexShader;
	delete pShadowShaderProgram;

	delete pScreenRenderTarget;
	delete pFullScreenRenderTarget;
	delete graphics;

	delete pTextureUnit;

	return Msg.wParam;
}

// sets the view, projection matrix as a shader parameter
void UpdateViewProjectionMatrix()
{
	// View and projection matrices for setting up camera view
	mini3d::utilites::Matrix4 viewMatrix;
	mini3d::utilites::Matrix4 projectionMatrix;

	mini3d::utilites::Vector3 eye(distance * cos(rotY) * cos((float)rotX), distance * sin(rotY), distance * cos(rotY) * sin(rotX));
	mini3d::utilites::Vector3 target(0,0,0);
	mini3d::utilites::Vector3 up(0,-1,0);

	// update camera
	mini3d::utilites::math3d::setuplookat(&viewMatrix._00, (float*)&eye, (float*)&target, (float*)&up);
	mini3d::utilites::math3d::BuildPerspProjMat(&projectionMatrix._00, (float)(3.1415f / 4.0f), (float)width/(float)height, 1.0f, 80.0f);
	mini3d::utilites::Matrix4 viewprojectionMatrix = viewMatrix * projectionMatrix;

	// set a shader parameter
	graphics->SetShaderParameterMatrix(0, &viewprojectionMatrix._00, 4, 4);
	graphics->SetShaderParameterFloat(9, &eye.x, 4);
}

// sets the view, projection matrix as a shader parameter
void UpdateLightMatrix()
{
	// View and projection matrices for shadow map generation
	mini3d::utilites::Matrix4 lightViewMatrix;
	mini3d::utilites::Matrix4 lightProjection;

	mini3d::utilites::Vector3 lightPos(10.0f * cos(lightAnimationProgress), -8.0f + sin(lightAnimationProgress * 2.1f), 10.0f * sin(lightAnimationProgress)); // cos(lightAnimationProgress) * sin(lightAnimationProgress)
	mini3d::utilites::Vector3 lightTarget(0,0,0);
	mini3d::utilites::Vector3 up(0,-1,0);

	// update camera
	mini3d::utilites::math3d::setuplookat(&lightViewMatrix._00, (float*)&lightPos, (float*)&lightTarget, (float*)&up);
	mini3d::utilites::math3d::BuildPerspProjMat(&lightProjection._00, (float)(3.1415f / 8.0f), 1, 1.0f, 40.0f);
	mini3d::utilites::Matrix4 lightViewProjectionMatrix = lightViewMatrix * lightProjection;


	// set a shader parameter
	graphics->SetShaderParameterMatrix(4, &lightViewProjectionMatrix._00, 4, 4);

	//lightPos.Normalize();
	graphics->SetShaderParameterFloat(8, &lightPos.x, 4);
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
			}
		break;
		default:
			// return the window message to the default window process
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
