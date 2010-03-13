#include "DX9VertexBuffer.h"
#include <d3d9.h>

DX9IndexBuffer::DX9IndexBuffer(DX9GraphicsService* pGraphicsService, void* pIndices, unsigned int count) : 
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pIndices(0), pIndexBuffer(0)
{
	SetIndices(pIndices, count);
	LoadResource();	
}

DX9IndexBuffer::~DX9IndexBuffer(void)
{
	UnloadResource();
	UnloadIndices();
	pGraphicsService->RemoveResource(this);
}

void DX9IndexBuffer::SetIndices(void* pIndices, unsigned int count)
{
	UnloadIndices();

	this->sizeInBytes = count * 4; // TODO: depends on index type

	this->pIndices = pIndices;

	isDirty = true;
}
void* DX9IndexBuffer::GetIndices(unsigned int& count)
{
	void* pReturnIndices = pIndices;
	count = GetIndexCount(); // TODO: depends on index type

	// reset the data because we are "removing it" when we do a GetVertices!
	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;

	return pReturnIndices;
}
unsigned int DX9IndexBuffer::GetIndexCount(void)
{
	return sizeInBytes / 4;
}
IDirect3DIndexBuffer9* DX9IndexBuffer::GetIndexBuffer(void)
{
	return pIndexBuffer;
}
void DX9IndexBuffer::UnloadIndices(void)
{
	if (pIndices != 0)
		delete pIndices;

	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;
}
void DX9IndexBuffer::LoadResource(void)
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

void DX9IndexBuffer::UnloadResource(void)
{
	if (pIndexBuffer != 0)
	{
		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}

	isDirty = true;
}
bool DX9IndexBuffer::GetIsDirty(void)
{
	return isDirty;
}
