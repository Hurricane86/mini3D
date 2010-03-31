
#include "../DX9VertexBuffer.h"
#include <d3d9.h>
#include "../../VertexDeclaration.h"

DX9VertexBuffer::DX9VertexBuffer(void) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), pVertexDeclaration(0), sizeInBytes(0)
{
}


DX9VertexBuffer::DX9VertexBuffer(DX9GraphicsService* pGraphicsService, void* pVertices, unsigned int count, const VertexDeclaration& vertexDeclaration) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), pVertexDeclaration(0), sizeInBytes(0)
{
	SetVertices(pVertices, count, vertexDeclaration);
	LoadResource();	
}

DX9VertexBuffer::~DX9VertexBuffer(void)
{
	UnloadResource();
	UnloadVertices();
	pGraphicsService->RemoveResource(this);
}

void DX9VertexBuffer::SetVertices(void* pVertices, unsigned int count, const VertexDeclaration& vertexDeclaration)
{
	UnloadVertices();

	SetVertexDeclaration(vertexDeclaration);

	int sizeInBytes = count * GetVertexSizeInBytes();

	this->pVertices = pVertices;

	this->sizeInBytes =  sizeInBytes;
	this->vertexDeclaration = vertexDeclaration;

	isDirty = true;
}
void DX9VertexBuffer::SetVertexDeclaration(const VertexDeclaration& vertexDeclaration)
{
	// if we already have a vertex declaration, release it from the graphics service pool
	//if (this->vertexDeclaration.GetCount() != 0)
		//pGraphicsService->ReleaseVertexDeclaration(vertexDeclaration);
	
	// pool the new one
	//pGraphicsService->PoolVertexDeclaration(vertexDeclaration);

	vertexSizeInBytes = 0;
	int count = vertexDeclaration.GetCount();
	for (int i = 0; i < count; i++)
	{
		switch (vertexDeclaration.GetVertexDataTypes(sizeInBytes)[i])
		{
				
		case vertexDeclaration.POSITION:
			vertexSizeInBytes += 12;
			break;
		case vertexDeclaration.COLOR:
			vertexSizeInBytes += 4;
			break;
		case vertexDeclaration.TEXTURECOORDINATE:
			vertexSizeInBytes += 8;
			break;
		}
	}
}
unsigned int DX9VertexBuffer::GetVertexSizeInBytes(void)
{
	return vertexSizeInBytes;
}

void DX9VertexBuffer::UnloadVertices(void)
{
	if (pVertices != 0)
		operator delete(pVertices);

	pVertices = 0;
	sizeInBytes = 0;
}
void* DX9VertexBuffer::GetVertices(unsigned int& count, VertexDeclaration& vertexDeclaration)
{
	void* pReturnVertices = pVertices;
	count = GetVertexCount();
	vertexDeclaration = this->vertexDeclaration;
	
	// reset the data because we are "removing it" when we do a GetVertices!
	pVertices = 0;
	sizeInBytes = 0;
	vertexDeclaration = VertexDeclaration();

	isDirty = true;

	return pReturnVertices;
}
unsigned int DX9VertexBuffer::GetVertexCount()
{
	return this->sizeInBytes / GetVertexSizeInBytes();
}
VertexDeclaration DX9VertexBuffer::GetVertexDeclaration()
{
	return vertexDeclaration;
}
IDirect3DVertexBuffer9* DX9VertexBuffer::GetVertexBuffer()
{
	return pVertexBuffer;
}
IDirect3DVertexDeclaration9* DX9VertexBuffer::GetVertexDeclarationBuffer()
{
	return pVertexDeclaration;
}
void DX9VertexBuffer::LoadResource(void)
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

void DX9VertexBuffer::UnloadResource(void)
{
	if (pVertexBuffer != 0)
	{
		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}

	isDirty = true;
}
bool DX9VertexBuffer::GetIsDirty(void)
{
	return isDirty;
}
