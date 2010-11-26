
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../D3D9VertexBuffer.h"
#include <d3d9.h>
#include <d3dx9.h>

mini3d::D3D9VertexShader::D3D9VertexShader(D3D9GraphicsService* pGraphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes, const IVertexShader::VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount) :
	pGraphicsService(pGraphicsService), pShaderBuffer(0)
{
	// Vertex shader data
	this->sizeInBytes = sizeInBytes;

	this->pShaderBytes = malloc(sizeInBytes);
	memcpy(this->pShaderBytes, pShaderBytes, sizeInBytes);

	// VertexDeclaration
	this->vertexDataCount = vertexDataCount;

	this->vertexDeclaration = new VertexDataType[vertexDataCount];
	memcpy(this->vertexDeclaration, vertexDeclaration, vertexDataCount * sizeof(VertexDataType));

	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::IVertexShader::VertexDataType* mini3d::D3D9VertexShader::GetVertexDeclaration(unsigned int& vertexDataCount) const
{ 
	vertexDataCount = this->vertexDataCount;
	unsigned int sizeInBytes = this->vertexDataCount * sizeof(VertexDataType);

	VertexDataType* pVertexDeclarationCopy = new IVertexShader::VertexDataType[sizeInBytes]; 
	memcpy(pVertexDeclarationCopy, vertexDeclaration, sizeInBytes);
	
	return pVertexDeclarationCopy;
};

mini3d::D3D9VertexShader::~D3D9VertexShader(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);

	free(pShaderBytes);
	delete[] vertexDeclaration;
}

void mini3d::D3D9VertexShader::LoadResource(void)
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
	D3DXCompileShader((LPCSTR)pShaderBytes, sizeInBytes, 0, 0, "main", "vs_2_0", 0, &buffer, &ppErroMessage, 0);
	
	if (ppErroMessage != 0)
	{
		OutputDebugStringA((char*)(ppErroMessage->GetBufferPointer()));
	}

	if( FAILED( pDevice->CreateVertexShader((DWORD*)buffer->GetBufferPointer(), &pShaderBuffer)))
	{
		isDirty = true;
		return;
	}

	buffer->Release();
	isDirty = false;

	// load the vertex declaration into the pool
	pGraphicsService->PoolVertexDeclaration(vertexDeclaration, vertexDataCount);
}


void mini3d::D3D9VertexShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		// if this is the currently loaded pixel shader, release it
		if (pGraphicsService->GetVertexShader() == this)
			pGraphicsService->SetVertexShader(0);

		pShaderBuffer->Release();
		pShaderBuffer = 0;
	}

	isDirty = true;

	// remove the vertex declaration from the pool
	pGraphicsService->ReleaseVertexDeclaration(vertexDeclaration, vertexDataCount);
}
