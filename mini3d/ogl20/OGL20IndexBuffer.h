
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20INDEXBUFFER_H
#define MINI3D_OGL20INDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "internal/IOGL20Resource.h"
#include "os/OSFunction.h"


namespace mini3d
{
class OGL20GraphicsService;
class OGL20IndexBuffer : public IIndexBuffer, public IOGL20Resource
{


public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20IndexBuffer(OGL20GraphicsService* graphicsService, const void* pIndices, const unsigned int& count, const DataType& dataType = INT_32, const CullMode& cullMode = CULL_COUNTERCLOCKWIZE);
	~OGL20IndexBuffer();


	// ::::: IIndexBuffer :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetIndices(unsigned int& sizeInBytes) const;
	virtual void SetIndices(const void* pIndices, const unsigned int& count, const DataType& dataType = INT_32, const CullMode& cullMode = CULL_COUNTERCLOCKWIZE);
	
	virtual void* Lock(unsigned int& sizeInBytes) const;
	virtual void Unlock(const bool& dataIsChanged);

	virtual unsigned int GetIndexCount() const { return indexCount; };
	virtual DataType GetDataType() const { return dataType; };

	virtual CullMode GetCullMode() const { return cullMode; };
	virtual void SetCullMode(const CullMode& cullMode) { this->cullMode = cullMode; };


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual GLuint GetIndexBuffer() { return pIndexBuffer; };
	

private:
	
	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadIndices();
	unsigned int GetBytesPerIndex();

private:
	
	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;
	OSFunctions* pOS;

	// Buffer
	GLuint pIndexBuffer;
	int bufferSizeInBytes;

	// Indices
	void* pIndices;
	
	// size of index buffer in bytes
	int sizeInBytes;

	// Number of indices in index list
	unsigned int indexCount;

	// Cullmode for this index buffer
	CullMode cullMode;

	// Data type of the indices
	DataType dataType;

	// Keps track of the state of the resource
	bool isDirty;

};
}

#endif
