
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_DX9VERTEXBUFFER_H
#define AURORA_DX9VERTEXBUFFER_H

#include "../IVertexBuffer.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9Resource.h"

namespace mini3d
{
class DX9VertexBuffer :	public IVertexBuffer, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::
	
	DX9VertexBuffer(DX9GraphicsService* pGraphicsService, const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes);
	~DX9VertexBuffer(void);


	// ::::: IVertexBuffer ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetVertices(unsigned int& sizeInBytes) const;
	virtual void SetVertices(const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes);

	virtual void* Lock(unsigned int& sizeInBytes) const;
	virtual void Unlock(const bool& dataIsChanged);

	virtual unsigned int GetVertexCount() const { return sizeInBytes / vertexSizeInBytes; };
	virtual unsigned int GetVertexSizeInBytes() const { return vertexSizeInBytes; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	IDirect3DVertexBuffer9* GetVertexBuffer() const { return pVertexBuffer; };

private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadVertices();

private:

	// ::::: Private Member Variables :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	// vertices
	void* pVertices;
	unsigned int sizeInBytes;
	unsigned int vertexSizeInBytes;
	
	// Buffer
	IDirect3DVertexBuffer9* pVertexBuffer;
	int bufferSizeInBytes;
	bool isDirty;




};
}

#endif
