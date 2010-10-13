
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20IndexBuffer.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20IndexBuffer::OGL20IndexBuffer(OGL20GraphicsService* pGraphicsService, const void* pIndices, const unsigned int& count, const DataType& dataType, const CullMode& cullMode) : 
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pIndices(0), pIndexBuffer(0)
{
	SetIndices(pIndices, count, dataType, cullMode);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20IndexBuffer::~OGL20IndexBuffer(void)
{
	UnloadResource();
	UnloadIndices();
	pGraphicsService->RemoveResource(this);
}

void* mini3d::OGL20IndexBuffer::GetIndices(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pIndicesCopy = malloc(sizeInBytes);
	memcpy(pIndicesCopy, pIndices, sizeInBytes);
	
	return pIndicesCopy;
}

void mini3d::OGL20IndexBuffer::SetIndices(const void* pIndices, const unsigned int& count, const DataType& dataType, const CullMode& cullMode)
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

void mini3d::OGL20IndexBuffer::UnloadIndices(void)
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

void* mini3d::OGL20IndexBuffer::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pIndices;
}

void mini3d::OGL20IndexBuffer::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
}

void mini3d::OGL20IndexBuffer::LoadResource(void)
{
    if (pIndices == 0 || sizeInBytes == 0)
    {
		UnloadResource();
        return;
    }

	PFNGLMAPBUFFERPROC glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	PFNGLUNMAPBUFFERPROC glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	PFNGLBUFFERDATAPROC glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	PFNGLGENBUFFERSPROC glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pIndexBuffer != 0 && bufferSizeInBytes != sizeInBytes)
	{
		UnloadResource();
	}

	// If it does not exist, create a new one
	if (pIndexBuffer == 0)
	{
		glGenBuffers(1, &pIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer);

		try {
	
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndices, GL_STATIC_DRAW);
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
	GLvoid* pBufferVertices = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(pBufferVertices, pIndices, sizeInBytes);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	// Clear the bound array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	bufferSizeInBytes = sizeInBytes;
	isDirty = false;
}

void mini3d::OGL20IndexBuffer::UnloadResource(void)
{
	if (pIndexBuffer != 0)
	{

		PFNGLDELETEBUFFERSPROC glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

		// if this is the currently loaded index buffer, release it
		if (pGraphicsService->GetIndexBuffer() == this)
			pGraphicsService->SetIndexBuffer(0);

		glDeleteBuffers(1, &pIndexBuffer); 
		pIndexBuffer = 0;
	}

	isDirty = true;
}

unsigned int mini3d::OGL20IndexBuffer::GetBytesPerIndex()
{
	switch(dataType)
	{
		case INT_16:
		return 2;
		case INT_32:
		return 4;
	}

	// TODO: Default case to avoid build warnings
	return 4;

}