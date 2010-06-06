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

mini3d::DX9VertexBuffer::DX9VertexBuffer(DX9GraphicsService* pGraphicsService, const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), sizeInBytes(0)
{
	SetVertices(pVertices, count, vertexSizeInBytes);
	pGraphicsService->AddResource(this);
}

mini3d::DX9VertexBuffer::~DX9VertexBuffer(void)
{
	UnloadResource();
	UnloadVertices();
	pGraphicsService->RemoveResource(this);
}
void* mini3d::DX9VertexBuffer::GetVertices(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pVerticesCopy = malloc(sizeInBytes);
	memcpy (pVerticesCopy, pVertices, sizeInBytes);
	
	return pVerticesCopy;
}
void mini3d::DX9VertexBuffer::SetVertices(const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes)
{
	UnloadVertices();

	this->vertexSizeInBytes = vertexSizeInBytes;
	sizeInBytes = count * vertexSizeInBytes;

	this->pVertices = malloc(sizeInBytes);
	memcpy(this->pVertices, pVertices, sizeInBytes);

	isDirty = true;
	LoadResource();
}

void mini3d::DX9VertexBuffer::UnloadVertices(void)
{
	if (pVertices != 0)
	{
		free(pVertices);

		pVertices = 0;
		sizeInBytes = 0;
		vertexSizeInBytes = 0;
	}
}

void* mini3d::DX9VertexBuffer::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pVertices;
}

void mini3d::DX9VertexBuffer::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
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
		// if this is the currently loaded vertex buffer, release it
		if (pGraphicsService->GetVertexBuffer() == this)
			pGraphicsService->SetVertexBuffer(0);

		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}

	isDirty = true;
}
