
#include "d3d9.h"
#include <d3dx9.h>

#include "mini3D/IGraphicsService.h"
#include "mini3D/DirectX9/DX9GraphicsService.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define Vector3 D3DXVECTOR3

struct VertexPCT
{
	float x,y,z;
	int color;
	float u,v;
};

VertexPCT vertices[] = {{-5.0f,-5.0f, 0.0f,   (DWORD)D3DCOLOR_ARGB(255,128,128,128),   0.0f, 0.0f},
						{ 5.0f,-5.0f, 0.0f,   (DWORD)D3DCOLOR_ARGB(255,128,128,128),   1.0f, 0.0f},
						{ 5.0f, 5.0f, 0.0f,   (DWORD)D3DCOLOR_ARGB(255,128,128,128),   1.0f, 1.0f},
						{-5.0f, 5.0f, 0.0f,   (DWORD)D3DCOLOR_ARGB(255,128,128,128),   0.0f, 1.0f}};

int indices[] = {0, 1, 2, 0, 2, 3};

char* ShaderBytesFromFile(wstring file, long& sizeInBytes)
{
	
	sizeInBytes = 0;

	fstream fileStream(file.c_str(), ios_base::binary | ios_base::in);
	if (fileStream)
	{
		fileStream.seekg(0, ios::end);
		long fileSize = fileStream.tellg();
		fileStream.seekg(0, ios::beg);

		char* shaderBytes = new char[fileSize];

		fileStream.read(shaderBytes, fileSize);
		fileStream.close();
		sizeInBytes = fileSize;
		return shaderBytes;
	}
	return 0;
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

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
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

	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	VertexDeclaration::VertexDataType vdt[] = {VertexDeclaration.POSITION, VertexDeclaration.COLOR, VertexDeclaration.TEXTURECOORDINATE};
	int size = sizeof(vdt);
	VertexDeclaration vd(vdt, sizeof(vdt));

	IGraphicsService* graphics = new DX9GraphicsService(GraphicsSettings(), (int)hWindow);
	IIndexBuffer* iBuffer = graphics->CreateIndexBuffer(indices, 6);
	IVertexBuffer* vBuffer = graphics->CreateVertexBuffer(vertices, 4, vd);

	int* pBitmap = (int*)malloc(512 * 512 * 4);
	for(int i = 0; i < 512 * 512; i++)
	{
		pBitmap[i] = 0xFFFFFFFF * ((i / 128) % 2);
	}
	ITexture* pTexture = graphics->CreateTexture(pBitmap, 512, 512);
	
	free(pBitmap);

	long sizeInBytes;
	char* shaderBytes = ShaderBytesFromFile(L"D:/Projects/Aurora/Source/Aurora/waterPixel.fxo", sizeInBytes);
	IPixelShader* pPixelShader = graphics->CreatePixelShader(shaderBytes, sizeInBytes);

	shaderBytes = ShaderBytesFromFile(L"D:/Projects/Aurora/Source/Aurora/waterVertex.fxo", sizeInBytes);
	IVertexShader* pVertexShader = graphics->CreateVertexShader(shaderBytes, sizeInBytes);

	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
		
	D3DXMatrixLookAtLH(&viewMatrix, &Vector3(0,2,20), &Vector3(0,0,0), &Vector3(0,-1,0));
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)(3.1415f / 4.0f), 1, 40.0f / 1000.0f, 40.0f);

	D3DXMATRIX viewProjectionMatrix = viewMatrix * projectionMatrix;
	D3DXMATRIX viewProjectionMatrixTranspose;
	D3DXMatrixTranspose(&viewProjectionMatrixTranspose, &viewProjectionMatrix);
	//D3DXMatrixMultiply(&viewProjectionMatrix, &viewMatrix, &projectionMatrix);

	graphics->SetIndexBuffer(iBuffer);
	graphics->SetVertexBuffer(vBuffer);
	graphics->SetPixelShader(pPixelShader);
	graphics->SetVertexShader(pVertexShader);
	graphics->SetTexture(pTexture, 0);
	graphics->BeginFrame();
	graphics->BeginDraw();
	graphics->SetShaderParameterFloat(0, &viewProjectionMatrixTranspose._11, 4);
	graphics->ClearRenderTarget(0xFFDDCCFF);
	graphics->ClearDepthStencil();
	graphics->Draw();
	graphics->EndDraw();
	graphics->EndFrame();

	MSG Msg;
	while(true)
	{
	    while(GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	
		return Msg.wParam;
	}



	return 1;
}

