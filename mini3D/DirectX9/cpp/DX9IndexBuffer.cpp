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

#include "../DX9IndexBuffer.h"
#include <d3d9.h>

mini3d::DX9IndexBuffer::DX9IndexBuffer(DX9GraphicsService* pGraphicsService, const void* pIndices, const unsigned int& count, const DataType& dataType, const CullMode& cullMode) : 
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pIndices(0), pIndexBuffer(0)
{
	SetIndices(pIndices, count, dataType, cullMode);
	pGraphicsService->AddResource(this);
}

mini3d::DX9IndexBuffer::~DX9IndexBuffer(void)
{
	UnloadResource();
	UnloadIndices();
	pGraphicsService->RemoveResource(this);
}

void* mini3d::DX9IndexBuffer::GetIndices(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pIndicesCopy = malloc(sizeInBytes);
	memcpy(pIndicesCopy, pIndices, sizeInBytes);
	
	return pIndicesCopy;
}

void mini3d::DX9IndexBuffer::SetIndices(const void* pIndices, const unsigned int& count, const DataType& dataType, const CullMode& cullMode)
{
	UnloadIndices();

	this->dataType = dataType;
	this->sizeInBytes = count * GetBytesPerIndex();
	this->pIndices = malloc(sizeInBytes);
	memcpy(this->pIndices, pIndices, sizeInBytes);

	this->indexCount = count;
	this->cullMode = cullMode;

	isDirty = true;
	LoadResource();	
}

void mini3d::DX9IndexBuffer::UnloadIndices()
{
	if (pIndices != 0)
	{
		free(pIndices);

		pIndices = 0;
		sizeInBytes = 0;

		cullMode = CULL_COUNTERCLOCKWIZE;
		dataType = INT_16;

		isDirty = true;
	}
}

void* mini3d::DX9IndexBuffer::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pIndices;
}

void mini3d::DX9IndexBuffer::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
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

	D3DFORMAT indexFormat;

	switch(dataType)
	{
		case INT_16:
			indexFormat = D3DFMT_INDEX16;
		break;
		case INT_32:
			indexFormat = D3DFMT_INDEX32;
		break;
	}	

	// If it does not exist, create a new one
	if (pIndexBuffer == 0)
	{
		if( FAILED( pDevice->CreateIndexBuffer(sizeInBytes, D3DUSAGE_WRITEONLY, indexFormat, D3DPOOL_DEFAULT, &pIndexBuffer, NULL ) ) )
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

void mini3d::DX9IndexBuffer::UnloadResource()
{
	if (pIndexBuffer != 0)
	{
		// if this is the currently loaded index buffer, release it
		if (pGraphicsService->GetIndexBuffer() == this)
			pGraphicsService->SetIndexBuffer(0);

		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}

	isDirty = true;
}

unsigned int mini3d::DX9IndexBuffer::GetBytesPerIndex()
{
	switch(dataType)
	{
		case INT_16:
		return 2;
		case INT_32:
		return 4;
	}		
}