
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20VertexBuffer.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20VertexBuffer::OGL20VertexBuffer(OGL20GraphicsService* pGraphicsService, const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), sizeInBytes(0)
{
	SetVertices(pVertices, count, vertexSizeInBytes);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20VertexBuffer::~OGL20VertexBuffer(void)
{
	UnloadResource();
	UnloadVertices();
	pGraphicsService->RemoveResource(this);
}

void* mini3d::OGL20VertexBuffer::GetVertices(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pVerticesCopy = malloc(sizeInBytes);
	memcpy (pVerticesCopy, pVertices, sizeInBytes);
	
	return pVerticesCopy;
}

void mini3d::OGL20VertexBuffer::SetVertices(const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes)
{
	UnloadVertices();

	this->vertexSizeInBytes = vertexSizeInBytes;
	sizeInBytes = count * vertexSizeInBytes;

	this->pVertices = malloc(sizeInBytes);
	memcpy(this->pVertices, pVertices, sizeInBytes);

	isDirty = true;
	LoadResource();
}

void mini3d::OGL20VertexBuffer::UnloadVertices(void)
{
	if (pVertices != 0)
	{
		free(pVertices);

		pVertices = 0;
		sizeInBytes = 0;
		vertexSizeInBytes = 0;
	}
}

void* mini3d::OGL20VertexBuffer::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pVertices;
}

void mini3d::OGL20VertexBuffer::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
}

void mini3d::OGL20VertexBuffer::LoadResource(void)
{

	PFNGLGENBUFFERSPROC glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	PFNGLBUFFERDATAPROC glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	PFNGLMAPBUFFERPROC glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	PFNGLUNMAPBUFFERPROC glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");

	/// Allocate buffer on the graphics card and add index data.
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
		glGenBuffers(1, &pVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer);

		try 
		{
			glBufferData(GL_ARRAY_BUFFER, sizeInBytes, pVertices, GL_STATIC_DRAW);
		}
		catch (GLuint error)
		{
			switch (error)
			{
			case GL_OUT_OF_MEMORY:
				isDirty = true;
				return;
				break;
			}
		}
	}

	// Lock the buffer to gain access to the vertices 
	GLvoid* pBufferVertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(pBufferVertices, pVertices, sizeInBytes);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	// clear the bound buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	bufferSizeInBytes = sizeInBytes;
	isDirty = false;
}

void mini3d::OGL20VertexBuffer::UnloadResource(void)
{
	if (pVertexBuffer != 0)
	{
		PFNGLDELETEBUFFERSPROC glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

		// if this is the currently loaded vertex buffer, release it
		if (pGraphicsService->GetVertexBuffer() == this)
			pGraphicsService->SetVertexBuffer(0);

		glDeleteBuffers(1, &pVertexBuffer); 
		pVertexBuffer = 0;
	}

	isDirty = true;
}
