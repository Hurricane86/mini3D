
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../D3D9VertexBuffer.h"
#include "../D3D9GraphicsService.h"
#include <d3d9.h>
#include <d3dx9.h>

mini3d::D3D9PixelShader::D3D9PixelShader(D3D9GraphicsService* pGraphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes) :
	pGraphicsService(pGraphicsService), pShaderBuffer(0), sizeInBytes(sizeInBytes)
{
	this->pShaderBytes = malloc(sizeInBytes);
	memcpy(this->pShaderBytes, pShaderBytes, sizeInBytes);

	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::D3D9PixelShader::~D3D9PixelShader(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);

	free(pShaderBytes);
}

void mini3d::D3D9PixelShader::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

	// If the buffer exists tear it down.
	if (pShaderBuffer != 0)
	{
		UnloadResource();
	}

	// compile the shader source
	ID3DXBuffer* buffer;
	LPD3DXBUFFER ppErroMessage;
	D3DXCompileShader((LPCSTR)pShaderBytes, sizeInBytes, 0, 0, "main", "ps_2_0", 0, &buffer, &ppErroMessage, 0);

	if (ppErroMessage != 0)
	{
		OutputDebugStringA((char*)(ppErroMessage->GetBufferPointer()));
	}

	if( FAILED( pDevice->CreatePixelShader((DWORD*)pShaderBytes, &pShaderBuffer)))
	{
		isDirty = true;
		return;
	}

	buffer->Release();

	isDirty = false;
}

void mini3d::D3D9PixelShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		// if this is the currently loaded pixel shader, release it
		if (pGraphicsService->GetPixelShader() == this)
			pGraphicsService->SetPixelShader(0);

		pShaderBuffer->Release();
		pShaderBuffer = 0;
	}

	isDirty = true;
}
