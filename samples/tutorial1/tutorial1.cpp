
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#include <math.h>
#include "..\..\mini3d.h"

#include "utilities/OSWindow.h"
#include "utilities/M3DMatrix.h"
#include "utilities/M3DVector.h"

#include "shaders.h"
#include "geometry.h"


// ----- FORWARD DECLARATIONS -------------------------------------------------
void WndMessage(utilities::OSWindow* window, utilities::OSWindow::WindowMessage message);
void UpdateViewProjectionMatrix();
void Render();
unsigned int* CreateTestBitmap(unsigned int width, unsigned int height);


// ----- DECLARE VARIABLES ----------------------------------------------------

// View and projection matrices for setting up camera view
M3DMatrix viewMatrixE;
M3DMatrix projectionMatrixE;

// state variables
bool exitApplication = false;

// Keeps track of mouse movement
int mouseX, mouseY;

// Keeps track of the view angle and camera distance
float rotX = 3.1415f / 4.0f; // three quater spin
float rotY = -3.1415f / 4.0f; // three quater spin
float distance = 10.0f; // medium distance :)


// ----- DECLARE GRAPHICS RESOURCES -------------------------------------------

// Render Window
utilities::OSWindow* window;

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
	

	// ----- CREATE GRAPHICS SERVICE ------------------------------------------
	
	//graphics = new mini3d::D3D9GraphicsService();
	graphics = new mini3d::OGL20GraphicsService();


	// ----- CREATE GRAPHICS RESOURCES ----------------------------------------

	// create an os window and attach a render target (mini3d does not have a default render target)
	window = new utilities::OSWindow(WndMessage, 800, 600);
	pWindowRenderTarget = graphics->CreateWindowRenderTarget(window->GetWindowHandle(), true, mini3d::IWindowRenderTarget::QUALITY_MINIMUM);

	// create index buffer
	iBuffer = graphics->CreateIndexBuffer(indices, 36);
	
	// create vertex buffer
	vBuffer = graphics->CreateVertexBuffer(vertices, 24, sizeof(VertexPCT));
	
	// create a 32bpp texture
	unsigned int* pBitmap = CreateTestBitmap(512, 256);
	pTexture = graphics->CreateBitmapTexture((void *)pBitmap, 512, 256); 
	delete(pBitmap);

	// Vertex declaration for the vertex shader
	mini3d::IVertexShader::VertexDataType vertexDeclaration[] = { mini3d::IVertexShader::POSITION_FLOAT4, mini3d::IVertexShader::COLOR_FLOAT4, mini3d::IVertexShader::TEXTURECOORDINATE_FLOAT2 };

	// create vertex shader
//	pVertexShader = graphics->CreateVertexShader(vertexShaderHLSL, strlen(vertexShaderHLSL), vertexDeclaration, 3);
	pVertexShader = graphics->CreateVertexShader(vertexShaderGLSL, strlen(vertexShaderGLSL), vertexDeclaration, 3);

	// create pixel shader
//	pPixelShader = graphics->CreatePixelShader(pixelShaderHLSL, strlen(pixelShaderHLSL));
	pPixelShader = graphics->CreatePixelShader(pixelShaderGLSL, strlen(pixelShaderGLSL));

	// create shader program

	pShaderProgram = graphics->CreateShaderProgram(pVertexShader, pPixelShader);

	// ----- SHOW THE WINDOW --------------------------------------------------

	// Show the window
	window->Show();

	// ----- MAIN LOOP --------------------------------------------------------

	// loop while the window is not closed
	while(exitApplication == false)
	{
		window->WaitForMessage();
	}


	// ----- DELETE RESOURCES -------------------------------------------------

	delete vBuffer;
	delete iBuffer;
	delete pTexture;
	delete pPixelShader;
	delete pVertexShader;
	delete pShaderProgram;
	delete pWindowRenderTarget;
	delete graphics;

	delete window;
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
	M3DVector eye(distance * cos(rotY) * cos(rotX), distance * sin(rotY), distance * cos(rotY) * sin(rotX));
	M3DVector target(0,0,0);
	M3DVector up(0,-1,0);

	float windowWidth = (float)window->GetWidth();
	float windowHeight = (float)window->GetHeight();

	// update camera
	M3DMatrix::LookAt(viewMatrixE, eye, target, up);
	M3DMatrix::PerspectiveProjection(projectionMatrixE, (float)(3.1416f / 4.0f), windowWidth/windowHeight, 40.0f / 100.0f, 80.0f);
	M3DMatrix viewProjectionMatrixE = viewMatrixE * projectionMatrixE;

	// set a shader parameter
	graphics->SetShaderParameterMatrix(0, viewProjectionMatrixE(0,0), 4, 4);
}

void WndMessage(utilities::OSWindow* window, utilities::OSWindow::WindowMessage message)
{

	switch(message)
	{
		case utilities::OSWindow::PAINT:
			Render();
		break;
		case utilities::OSWindow::CLOSED:
			exitApplication = true;
		break;
		case utilities::OSWindow::DESTROYED:
			exitApplication = true;
		break;
		case utilities::OSWindow::SIZE:
			Render();
		break;
		case utilities::OSWindow::MOUSE_LEFT_DOWN:
			mouseX = window->GetMouseX();
			mouseY = window->GetMouseY();
		break;
		case utilities::OSWindow::KEY_DOWN:
			if ((window->GetKey() & window->VKC_F12) == window->VKC_F12)
			{
				if (pWindowRenderTarget->GetScreenState() == mini3d::IWindowRenderTarget::SCREEN_STATE_WINDOWED)
					pWindowRenderTarget->SetScreenStateFullscreen(800, 600);
				else
					pWindowRenderTarget->SetScreenStateWindowed();

				Render();
			}
		break;
		case utilities::OSWindow::MOUSE_MOVE:
			if (window->GetLeftMouseDown() == true)
			{
				rotX += (window->GetMouseX() - mouseX) / 100.0f;
				rotY -= (window->GetMouseY() - mouseY) / 100.0f;

				mouseX = window->GetMouseX();
				mouseY = window->GetMouseY();
				Render();
			}
		break;
		case utilities::OSWindow::MOUSE_LEFT_UP:
		break;
		case utilities::OSWindow::MOUSE_WHEEL:
			distance -= (float)window->GetMouseWheelDelta() * 0.01f;
			Render();
		break;
	}
}

// Creates a nice colorful bitmap of the requested size
unsigned int* CreateTestBitmap(unsigned int width, unsigned int height)
{
	unsigned int* pBitmap = new unsigned int[width * height];

	for (unsigned int x = 0; x < width; x++)
		for (unsigned int y = 0; y < height; y++)
			pBitmap[x + y * width] =	((x -  2 * y) % 255 << 24) + // RED
										(((255 - y - x / 2) % 255) << 16) + // GREEN
										((255 - (x + y)) % 255 << 8) + // BLUE
										255; // ALPHA

	return pBitmap;
}
