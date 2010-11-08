
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include <math.h>
#include "../../mini3d/utilities/OSWindow.h"
#include "../../mini3d.h"

//#include <Windows.h>

// ----- FORWARD DECLARATIONS -------------------------------------------------
void WndMessage(mini3d::utilites::OSWindow* window, mini3d::utilites::OSWindow::WindowMessage message);
void UpdateViewProjectionMatrix();
void Render();
unsigned int* CreateNiceBitmap(unsigned int width, unsigned int height);

// ----- GEOMETRY DATA --------------------------------------------------------

// Vertex Data Structure
struct VertexPCT { float x,y,z,w;  float r,g,b,a;  float u,v; };

// Vertex array
VertexPCT vertices[] = {
	// Front
	{-1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.5f, 0.0f}, // Corner 0
	{ 1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f, 0.75f, 0.0f}, // Corner 1
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.75f, 0.5f}, // Corner 2
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.5f, 0.5f}, // Corner 3
						
	// Back
	{ 1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.5f, 0.5f}, // Corner 4
	{-1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f, 0.75f, 0.5f}, // Corner 5
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.75f, 1.0f}, // Corner 6
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.5f, 1.0f}, // Corner 7

	// Right
	{ 1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f, 0.25f, 0.5f}, // Corner 1
	{ 1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.5f, 0.5f}, // Corner 4
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.5f, 1.0f}, // Corner 7
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.25f, 1.0f}, // Corner 2

	// Left
	{-1.0f, -1.0f, -1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f, 0.25f, 0.0f}, // Corner 5
	{-1.0f, -1.0f,  1.0f, 1.0f,  0.9f, 0.9f, 0.9f, 1.0f,  0.5f, 0.0f}, // Corner 0
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f,  0.5f, 0.5f}, // Corner 3
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.25f, 0.5f}, // Corner 6

	// Top
	{-1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f}, // Corner 5
	{ 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f}, // Corner 4
	{ 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.5f}, // Corner 1
	{-1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.5f}, // Corner 0

	//Bottom
	{ 1.0f,  1.0f, -1.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.5f}, // Corner 7
	{-1.0f,  1.0f, -1.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f, 0.25f, 0.5f}, // Corner 6
	{-1.0f,  1.0f,  1.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f, 0.25f, 1.0f}, // Corner 3
	{ 1.0f,  1.0f,  1.0f, 1.0f,  0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f}  // Corner 2
};

// Index array
unsigned int indices[] = {0, 1, 2, 0, 2, 3,    4, 5, 6, 4, 6, 7,    8, 9, 10, 8, 10, 11,    12, 13, 14, 12, 14, 15,    16, 17, 18, 16, 18, 19,    20, 21, 22, 20, 22, 23};

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

// ----- DECLARE GRAPHICS RESOURCES -------------------------------------------

// Graphics Service
mini3d::IGraphicsService* graphics;

// Graphics Resources
mini3d::IWindowRenderTarget* pWindowRenderTarget;

mini3d::IIndexBuffer* iBuffer;
mini3d::IVertexBuffer* vBuffer;

mini3d::IBitmapTexture* pTexture;

mini3d::IVertexShader* pVertexShader;
mini3d::IPixelShader* pPixelShader;
mini3d::IShaderProgram* pShaderProgram;


// Tutorial Application
#ifdef _WIN32
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
#endif
#ifdef __linux
int main()
#endif
{
	
	// ----- CREATE A WINDOW --------------------------------------------------

	// create a window
	mini3d::utilites::OSWindow window(WndMessage, 640, 480);
	int hWindow = window.GetWindowHandle();
	window.Show();

	// ----- CREATE GRAPHICS SERVICE ------------------------------------------
	
	//graphics = new mini3d::D3D9GraphicsService();
	graphics = new mini3d::OGL20GraphicsService();

	// ----- CREATE GRAPHICS RESOURCES ----------------------------------------

	// create a render target (mini3d does not have a default render target)
	pWindowRenderTarget = graphics->CreateWindowRenderTarget(hWindow, true, mini3d::IWindowRenderTarget::QUALITY_MINIMUM);

	// create index buffer
	iBuffer = graphics->CreateIndexBuffer(indices, 36);
	
	// create vertex buffer
	vBuffer = graphics->CreateVertexBuffer(vertices, 24, sizeof(VertexPCT));
	
	// create texture
	void* pBitmap = (void *)CreateNiceBitmap(128, 64);
	// create a nice 32bpp texture
	pTexture = graphics->CreateBitmapTexture(pBitmap, 128, 64); 
	operator delete(pBitmap);

	unsigned int sizeInBytes;
	
	// create vertex shader
	char* shaderBytes = mini3d::utilites::BinaryFileReader::ReadBytesFromFile((char *)"shaders/glsl/vertexshader.glsl", sizeInBytes);
	pVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes, vertexDeclaration, 3);
	delete shaderBytes;

	// create pixel shader
	char* shaderBytes2 = mini3d::utilites::BinaryFileReader::ReadBytesFromFile((char *)"shaders/glsl/pixelshader.glsl", sizeInBytes);
	pPixelShader = graphics->CreatePixelShader(shaderBytes2, sizeInBytes);
	delete shaderBytes2;


	// ----- VARIABLES FOR RENDER LOOP ----------------------------------------
	
	// create shader program
	pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);


	// ----- CONFIIGURE GRAPHICS PIPELINE -------------------------------------

	// set render prarameters
	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetShaderProgram(pShaderProgram);

	graphics->SetRenderTarget(pWindowRenderTarget);

	// Set the Texture
	graphics->SetTexture(pTexture, 0);

	// Render to the Screen
	Render();

	// ----- MAIN LOOP --------------------------------------------------------

	// loop while the window is not closed
#ifdef _WIN32
	MSG Msg;
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		// window message stuff
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
#endif
#ifdef __linux
	window.WaitForMessage();
#endif

	// ----- DELETE RESOURCES -------------------------------------------------

	delete vBuffer;
	delete iBuffer;
	delete pTexture;
	delete pPixelShader;
	delete pVertexShader;
	delete pShaderProgram;
	delete pWindowRenderTarget;
	delete graphics;


#ifdef _WIN32
	return Msg.wParam;
#endif
}

void Render()
{
	// set render prarameters
	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetShaderProgram(pShaderProgram);

	graphics->SetRenderTarget(pWindowRenderTarget);

	// Set the Texture
	graphics->SetTexture(pTexture, 0);

	// Set the ViewProjection matrix
	UpdateViewProjectionMatrix();

	// clear render target with color
	graphics->Clear(0.5,0.5,0.5,1.0);

	// draw the scene to the renderTargetTexture
	graphics->Draw();

	// do a flip
	pWindowRenderTarget->Display();
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

void WndMessage(mini3d::utilites::OSWindow* window, mini3d::utilites::OSWindow::WindowMessage message)
{

	switch(message)
	{
		case mini3d::utilites::OSWindow::PAINT:
			Render();
		break;
		case mini3d::utilites::OSWindow::CLOSED:

		break;
		case mini3d::utilites::OSWindow::DESTROYED:
#ifdef _WIN32 
			PostQuitMessage(0); 
#endif
			break;
		case mini3d::utilites::OSWindow::SIZE:
			width = window->GetWidth();
			height = window->GetHeight();
			pWindowRenderTarget->SetScreenStateWindowed();
			Render();
		break;
		case mini3d::utilites::OSWindow::MOUSE_LEFT_DOWN:
			mouseX = window->GetMouseX();
			mouseY = window->GetMouseY();
		break;
		case mini3d::utilites::OSWindow::KEY_DOWN:
#ifdef _WIN32
			if ((window->GetKey() & VK_F12) == VK_F12)
			{
				fullscreen = !fullscreen;

				// Set the screenrendertarget to the correct one
				if (fullscreen == true)
					pWindowRenderTarget->SetScreenStateFullscreen(0,0);
				else
					pWindowRenderTarget->SetScreenStateWindowed();
			}
			Render();
#endif
		break;
		case mini3d::utilites::OSWindow::MOUSE_MOVE:
			if (window->GetLeftMouseDown() == true)
			{
				int x = window->GetMouseX(); 
				int y = window->GetMouseY();

				rotX += (x - mouseX) / 100.0f;
				rotY -= (y - mouseY) / 100.0f;

				if (rotY > 3.1415f / 2.0f)
					rotY = 3.1415f / 2.0f;
				else if (rotY < -3.1415f / 2.0f)
					rotY = -3.1415f / 2.0f;
								
				mouseX = x;
				mouseY = y;
				Render();
			}
		break;
		case mini3d::utilites::OSWindow::MOUSE_LEFT_UP:
		break;
		case mini3d::utilites::OSWindow::MOUSE_WHEEL:
			{
				int zWheel = window->GetMouseWheelDelta();
				float zWheelf = (float)zWheel;
				distance -=  (float)zWheel * 0.01f;
			}
			Render();
		break;
	}
}

unsigned int* CreateNiceBitmap(unsigned int width, unsigned int height)
{
	float pi = 3.1416f;
	unsigned int* pBitmap = new unsigned int[width * height];
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			// set RGBA for pixel
			pBitmap[x + y * 128] =  ((unsigned int)(64 + 63 * (cos(x * 2 * pi / (float)width) + cos(y * 2 * pi / (float)height))) << 24) + 
									((2 * (x % (width / 4)) + 128) << 16) + 
									((2 * (y % (height / 2)) + 192) << 8) + 
									255; 
		}
	}
	return pBitmap;
}
