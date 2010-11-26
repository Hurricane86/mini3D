
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#ifndef MINI3D_D3D9VERTEXBUFFER_H
#define MINI3D_D3D9VERTEXBUFFER_H

#include "../mini3d/IVertexBuffer.h"
#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"

namespace mini3d
{
class D3D9VertexBuffer :	public IVertexBuffer, public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::
	
	D3D9VertexBuffer(D3D9GraphicsService* pGraphicsService, const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes);
	~D3D9VertexBuffer(void);


	// ::::: IVertexBuffer ::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void* GetVertices(unsigned int& sizeInBytes) const;
	virtual void SetVertices(const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes);

	virtual void* Lock(unsigned int& sizeInBytes) const;
	virtual void Unlock(const bool& dataIsChanged);

	virtual unsigned int GetVertexCount() const { return sizeInBytes / vertexSizeInBytes; };
	virtual unsigned int GetVertexSizeInBytes() const { return vertexSizeInBytes; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

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
	D3D9GraphicsService* pGraphicsService;

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
#endif
