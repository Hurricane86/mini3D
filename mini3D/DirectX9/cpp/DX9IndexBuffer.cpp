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

mini3d::DX9IndexBuffer::DX9IndexBuffer(DX9GraphicsService* pGraphicsService, void* pIndices, unsigned int count) : 
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pIndices(0), pIndexBuffer(0)
{
	SetIndices(pIndices, count);
	LoadResource();	
}

mini3d::DX9IndexBuffer::~DX9IndexBuffer(void)
{
	UnloadResource();
	UnloadIndices();
	pGraphicsService->RemoveResource(this);
}

void mini3d::DX9IndexBuffer::SetIndices(void* pIndices, unsigned int count)
{
	UnloadIndices();

	this->sizeInBytes = count * 4; // TODO: depends on index type

	this->pIndices = pIndices;

	isDirty = true;
}
void* mini3d::DX9IndexBuffer::GetIndices(unsigned int& count)
{
	void* pReturnIndices = pIndices;
	count = GetIndexCount(); // TODO: depends on index type

	// reset the data because we are "removing it" when we do a GetVertices!
	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;

	return pReturnIndices;
}
unsigned int mini3d::DX9IndexBuffer::GetIndexCount(void)
{
	return sizeInBytes / 4;
}
IDirect3DIndexBuffer9* mini3d::DX9IndexBuffer::GetIndexBuffer(void)
{
	return pIndexBuffer;
}
void mini3d::DX9IndexBuffer::UnloadIndices(void)
{
	if (pIndices != 0)
		operator delete(pIndices);

	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;
}
void mini3d::DX9IndexBuffer::LoadResource(void)
{
	/// Allocate buffer on the graphics card and add index data.
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

    if (pIndices == 0 || sizeInBytes == 0)
    {
		UnloadResource();
        return;
    }

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pIndexBuffer != 0 && bufferSizeInBytes != sizeInBytes)
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pIndexBuffer == 0)
	{
		if( FAILED( pDevice->CreateIndexBuffer(sizeInBytes, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIndexBuffer, NULL ) ) )
		{
			isDirty = true;
			return;
		}
	}

	// Lock the buffer to gain access to the Indices 
	void* pBufferIndices;

	if(FAILED(pIndexBuffer->Lock(0, 0, &pBufferIndices, 0)))
	{
		isDirty = true;
		return;
	}

	memcpy(pBufferIndices, pIndices, sizeInBytes);
	pIndexBuffer->Unlock();

	bufferSizeInBytes = sizeInBytes;
	isDirty = false;
}

void mini3d::DX9IndexBuffer::UnloadResource(void)
{
	if (pIndexBuffer != 0)
	{
		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}

	isDirty = true;
}
bool mini3d::DX9IndexBuffer::GetIsDirty(void)
{
	return isDirty;
}
