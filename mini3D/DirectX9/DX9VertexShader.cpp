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

#include "DX9VertexBuffer.h"
#include <d3d9.h>


DX9VertexShader::DX9VertexShader(DX9GraphicsService* pGraphicsService, void* pShaderBytes, unsigned int sizeInBytes, const VertexDeclarationVector& vertexDeclaration) :
	pGraphicsService(pGraphicsService), pShaderBytes(0), pShaderBuffer(0)
{
	SetVertexShader(pShaderBytes, sizeInBytes, vertexDeclaration);
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

void DX9VertexShader::SetVertexShader(void* pShaderBytes, unsigned int sizeInBytes, const VertexDeclarationVector& vertexDeclaration)
{
	UnloadVertexShader();
	SetVertexDeclaration(vertexDeclaration);
	
	this->pShaderBytes = pShaderBytes;
	this->sizeInBytes = sizeInBytes;

	isDirty = true;
}

void DX9VertexShader::SetVertexDeclaration(const VertexDeclarationVector& vertexDeclaration)
{
	// if we already have a vertex declaration, release it from the graphics service pool
	if (this->vertexDeclaration.size() != 0)
		pGraphicsService->ReleaseVertexDeclaration(vertexDeclaration);

	this->vertexDeclaration = vertexDeclaration;

	// pool the new one
	pGraphicsService->PoolVertexDeclaration(vertexDeclaration);
}

IVertexShader::VertexDeclarationVector DX9VertexShader::GetVertexDeclaration()
{
	return vertexDeclaration;	
}

void DX9VertexShader::UnloadVertexShader(void)
{
	operator delete(pShaderBytes);
	pGraphicsService->ReleaseVertexDeclaration(vertexDeclaration);
	pShaderBytes = 0;
	sizeInBytes = 0;
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