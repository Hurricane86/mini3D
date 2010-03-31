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

#include "../DX9VertexBuffer.h"
#include <d3d9.h>

DX9PixelShader::DX9PixelShader(DX9GraphicsService* pGraphicsService, void* pShaderBytes, unsigned int sizeInBytes) :
	pGraphicsService(pGraphicsService), pShaderBytes(0), pShaderBuffer(0)
{
	SetPixelShader(pShaderBytes, sizeInBytes);
	LoadResource();
}

DX9PixelShader::~DX9PixelShader(void)
{
	UnloadResource();
	UnloadPixelShader();
	pGraphicsService->RemoveResource(this);
}

void* DX9PixelShader::GetPixelShader(unsigned int& sizeInBytes)
{
	void* pReturnShaderBytes = pShaderBytes;
	sizeInBytes = this->sizeInBytes;

	// reset the data because we are "removing it" when we do a GetPixelShader!
	this->pShaderBytes = 0;
	this->sizeInBytes = 0;
	this->isDirty = true;


	return pReturnShaderBytes;
}
void DX9PixelShader::SetPixelShader(void* pShaderBytes, unsigned int sizeInBytes)
{
	UnloadPixelShader();
	
	this->pShaderBytes = pShaderBytes;
	this->sizeInBytes = sizeInBytes;

	isDirty = true;
}
IDirect3DPixelShader9* DX9PixelShader::GetPixelShaderBuffer(void)
{
	return pShaderBuffer;
}
void DX9PixelShader::UnloadPixelShader(void)
{
	if (pShaderBytes != 0)
		operator delete(pShaderBytes);

	pShaderBytes = 0;
	sizeInBytes = 0;
}
unsigned int DX9PixelShader::GetSizeInBytes()
{
	return sizeInBytes;
}
void DX9PixelShader::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

	// If the buffer exists tear it down.
	if (pShaderBuffer != 0)
	{
		UnloadResource();
	}

	if( FAILED( pDevice->CreatePixelShader((DWORD*)pShaderBytes, &pShaderBuffer)))
	{
		isDirty = true;
		return;
	}

	isDirty = false;
}

void DX9PixelShader::UnloadResource(void)
{
	if (pShaderBuffer != 0)
	{
		pShaderBuffer->Release();
		pShaderBuffer = 0;
	}

	isDirty = true;
}
bool DX9PixelShader::GetIsDirty(void)
{
	return isDirty;
}
