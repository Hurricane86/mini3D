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

#include "../OGL20IndexBuffer.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20IndexBuffer::OGL20IndexBuffer(OGL20GraphicsService* pGraphicsService, void* pIndices, unsigned int count) : 
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pIndices(0), pIndexBuffer(0)
{
	SetIndices(pIndices, count);
	LoadResource();	
	pGraphicsService->AddResource(this);
}

mini3d::OGL20IndexBuffer::~OGL20IndexBuffer(void)
{
	UnloadResource();
	UnloadIndices();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20IndexBuffer::SetIndices(void* pIndices, unsigned int count)
{
	UnloadIndices();

	this->sizeInBytes = count * 4; // TODO: depends on index type
	this->pIndices = pIndices;

	isDirty = true;
}
void* mini3d::OGL20IndexBuffer::GetIndices(unsigned int& count)
{
	void* pReturnIndices = pIndices;
	count = GetIndexCount(); // TODO: depends on index type

	// reset the data because we are "removing it" when we do a GetVertices!
	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;

	return pReturnIndices;
}
unsigned int mini3d::OGL20IndexBuffer::GetIndexCount(void)
{
	return sizeInBytes / 4;
}
GLuint mini3d::OGL20IndexBuffer::GetIndexBuffer(void)
{
	return pIndexBuffer;
}
void mini3d::OGL20IndexBuffer::UnloadIndices(void)
{
	if (pIndices != 0)
		operator delete(pIndices);

	pIndices = 0;
	sizeInBytes = 0;

	isDirty = true;
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
			//GL_OUT_OF_MEMORY
			isDirty = true;
			return;
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
bool mini3d::OGL20IndexBuffer::GetIsDirty(void)
{
	return isDirty;
}
