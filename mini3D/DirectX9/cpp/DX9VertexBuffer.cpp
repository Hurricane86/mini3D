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

mini3d::DX9VertexBuffer::DX9VertexBuffer(void) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), pVertexDeclaration(0), sizeInBytes(0)
{
}


mini3d::DX9VertexBuffer::DX9VertexBuffer(DX9GraphicsService* pGraphicsService, void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), pVertexDeclaration(0), sizeInBytes(0)
{
	SetVertices(pVertices, count, vertexDeclaration);
	LoadResource();	
}

mini3d::DX9VertexBuffer::~DX9VertexBuffer(void)
{
	UnloadResource();
	UnloadVertices();
	pGraphicsService->RemoveResource(this);
}

void mini3d::DX9VertexBuffer::SetVertices(void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration)
{
	UnloadVertices();

	SetVertexDeclaration(vertexDeclaration);

	int sizeInBytes = count * GetVertexSizeInBytes();

	this->pVertices = pVertices;

	this->sizeInBytes =  sizeInBytes;
	this->vertexDeclaration = vertexDeclaration;

	isDirty = true;
}
void mini3d::DX9VertexBuffer::SetVertexDeclaration(const VertexDeclarationVector& vertexDeclaration)
{
	// if we already have a vertex declaration, release it from the graphics service pool
	//if (this->vertexDeclaration.GetCount() != 0)
		//pGraphicsService->ReleaseVertexDeclaration(vertexDeclaration);
	
	// pool the new one
	//pGraphicsService->PoolVertexDeclaration(vertexDeclaration);

	vertexSizeInBytes = 0;
	int count = vertexDeclaration.size();
	for (int i = 0; i < count; i++)
	{
		switch (vertexDeclaration[i])
		{
				
		case IVertexBuffer::POSITION:
			vertexSizeInBytes += 12;
			break;
		case IVertexBuffer::COLOR:
			vertexSizeInBytes += 4;
			break;
		case IVertexBuffer::TEXTURECOORDINATE:
			vertexSizeInBytes += 8;
			break;
		}
	}
}
unsigned int mini3d::DX9VertexBuffer::GetVertexSizeInBytes(void)
{
	return vertexSizeInBytes;
}

void mini3d::DX9VertexBuffer::UnloadVertices(void)
{
	if (pVertices != 0)
		operator delete(pVertices);

	pVertices = 0;
	sizeInBytes = 0;
}
void* mini3d::DX9VertexBuffer::GetVertices(unsigned int& count, VertexDeclarationVector& vertexDeclaration)
{
	void* pReturnVertices = pVertices;
	count = GetVertexCount();
	vertexDeclaration = this->vertexDeclaration;
	
	// reset the data because we are "removing it" when we do a GetVertices!
	pVertices = 0;
	sizeInBytes = 0;
	vertexDeclaration = VertexDeclarationVector();

	isDirty = true;

	return pReturnVertices;
}
unsigned int mini3d::DX9VertexBuffer::GetVertexCount()
{
	return this->sizeInBytes / GetVertexSizeInBytes();
}
mini3d::IVertexBuffer::VertexDeclarationVector mini3d::DX9VertexBuffer::GetVertexDeclaration()
{
	return vertexDeclaration;
}
IDirect3DVertexBuffer9* mini3d::DX9VertexBuffer::GetVertexBuffer()
{
	return pVertexBuffer;
}
IDirect3DVertexDeclaration9* mini3d::DX9VertexBuffer::GetVertexDeclarationBuffer()
{
	return pVertexDeclaration;
}
void mini3d::DX9VertexBuffer::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

    if (pVertices == 0 || sizeInBytes == 0)
    {
		UnloadResource();
        return;
    }

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pVertexBuffer != 0 && bufferSizeInBytes != sizeInBytes)
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pVertexBuffer == 0)
	{
		if( FAILED( pDevice->CreateVertexBuffer(sizeInBytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pVertexBuffer, NULL ) ) )
		{
			isDirty = true;
			return;
		}
	}

	// Lock the buffer to gain access to the vertices 
	void* pBufferVertices;

	if(FAILED(pVertexBuffer->Lock(0, 0, &pBufferVertices, 0)))
	{
		isDirty = true;
		return;
	}

	memcpy(pBufferVertices, pVertices, sizeInBytes);
	pVertexBuffer->Unlock();

	bufferSizeInBytes = sizeInBytes;
	isDirty = false;
}

void mini3d::DX9VertexBuffer::UnloadResource(void)
{
	if (pVertexBuffer != 0)
	{
		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}

	isDirty = true;
}
bool mini3d::DX9VertexBuffer::GetIsDirty(void)
{
	return isDirty;
}
