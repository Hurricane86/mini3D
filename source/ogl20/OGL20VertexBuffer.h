
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20VERTEXBUFFER_H
#define MINI3D_OGL20VERTEXBUFFER_H

#include "../mini3d/IVertexBuffer.h"
#include "internal/IOGL20Resource.h"
#include "oglwrapper/OGLWrapper.h"

namespace mini3d
{
class OGL20GraphicsService;
class OGL20VertexBuffer :	public IVertexBuffer, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20VertexBuffer(OGL20GraphicsService* pGraphicsService, const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes);
	~OGL20VertexBuffer(void);


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

	GLuint GetVertexBuffer() const { return pVertexBuffer; };


private:

	// ::::: Private Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadVertices(void);


private:

	// ::::: Private Member Variables :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;
	OGLWrapper* pOGLWrapper;

	// vertices
	void* pVertices;
	unsigned int sizeInBytes;
	unsigned int vertexSizeInBytes;
	
	// Buffer
	GLuint pVertexBuffer;
	int bufferSizeInBytes;
	bool isDirty;

};
}

#endif
