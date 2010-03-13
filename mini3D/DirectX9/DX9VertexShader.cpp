#include "DX9VertexBuffer.h"
#include <d3d9.h>


DX9VertexShader::DX9VertexShader(DX9GraphicsService* pGraphicsService, void* pShaderBytes, unsigned int sizeInBytes) :
	pGraphicsService(pGraphicsService), pShaderBytes(0), pShaderBuffer(0)
{
	SetVertexShader(pShaderBytes, sizeInBytes);
	LoadResource();
}

DX9VertexShader::~DX9VertexShader(void)
{
	UnloadResource();
	UnloadVertexShader();
	pGraphicsService->RemoveResource(this);
}

void* DX9VertexShader::GetVertexShader(unsigned int& sizeInBytes)
{
	void* pReturnShaderBytes = pShaderBytes;
	sizeInBytes = this->sizeInBytes;

	// reset the data because we are "removing it" when we do a GetVertexShader!
	this->pShaderBytes = 0;
	this->sizeInBytes = 0;
	this->isDirty = true;

	return pReturnShaderBytes;
}
void DX9VertexShader::SetVertexShader(void* pShaderBytes, unsigned int sizeInBytes)
{
	UnloadVertexShader();
	
	this->pShaderBytes = pShaderBytes;
	this->sizeInBytes = sizeInBytes;

	isDirty = true;
}
void DX9VertexShader::UnloadVertexShader(void)
{
	if (pShaderBytes != 0)
		delete pShaderBytes;

	pShaderBytes = 0;
	sizeInBytes = 0;
}
unsigned int DX9VertexShader::GetSizeInBytes()
{
	return sizeInBytes;
}
IDirect3DVertexShader9* DX9VertexShader::GetVertexShaderBuffer(void)
{
	return pShaderBuffer;
}
void DX9VertexShader::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

	// If the buffer exists tear it down.
	if (pShaderBuffer != 0)
	{
		UnloadResource();
	}

	if( FAILED( pDevice->CreateVertexShader((DWORD*)pShaderBytes, &pShaderBuffer)))
	{
		isDirty = true;
		return;
	}

	isDirty = false;
}

void DX9VertexShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		pShaderBuffer->Release();
		pShaderBuffer = 0;
	}

	isDirty = true;
}
bool DX9VertexShader::GetIsDirty(void)
{
	return isDirty;
}