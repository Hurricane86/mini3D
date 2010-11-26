
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_D3D9INDEXBUFFER_H
#define MINI3D_D3D9INDEXBUFFER_H

#include "../mini3d/IIndexBuffer.h"
#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"

namespace mini3d
{
class D3D9IndexBuffer : public IIndexBuffer, public ID3D9Resource
{

public:
	
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::
	
	D3D9IndexBuffer(D3D9GraphicsService* graphicsService, const void* pIndices, const unsigned int& count, const DataType& dataType = INT_32, const CullMode& cullMode = CULL_COUNTERCLOCKWIZE);
	~D3D9IndexBuffer();


	// ::::: IIndexBuffer :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetIndices(unsigned int& sizeInBytes) const;
	virtual void SetIndices(const void* pIndices, const unsigned int& count, const DataType& dataType = INT_32, const CullMode& cullMode = CULL_COUNTERCLOCKWIZE);

	virtual void* Lock(unsigned int& sizeInBytes) const;
	virtual void Unlock(const bool& dataIsChanged);

	virtual unsigned int GetIndexCount() const { return indexCount; };
	virtual DataType GetDataType() const { return dataType; };

	virtual CullMode GetCullMode() const { return cullMode; };
	virtual void SetCullMode(const CullMode& cullMode) { this->cullMode = cullMode; };
	

	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };

	
	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IDirect3DIndexBuffer9* GetIndexBuffer() { return pIndexBuffer; };


private:
	
	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadIndices(void);
	unsigned int GetBytesPerIndex();

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	D3D9GraphicsService* pGraphicsService;

	// Direct3D9 Index Buffer
	IDirect3DIndexBuffer9* pIndexBuffer;

	// Indices
	void* pIndices;

	// size of index buffer in bytes
	unsigned int sizeInBytes;

	// Number of indices in index list
	unsigned int indexCount;

	// Size of the allocated Direct3D9 Buffer in bytes
	int bufferSizeInBytes;

	// Cullmode for this index buffer
	CullMode cullMode;

	// Data type of the indices
	DataType dataType;

	// Keps track of the state of the resource
	bool isDirty;


};
}

#endif
#endif
