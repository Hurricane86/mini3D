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

#include "../OGL20VertexBuffer.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20VertexBuffer::OGL20VertexBuffer(void) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), sizeInBytes(0)
{
	pGraphicsService->AddResource(this);
}


mini3d::OGL20VertexBuffer::OGL20VertexBuffer(OGL20GraphicsService* pGraphicsService, void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration) :
	pGraphicsService(pGraphicsService), bufferSizeInBytes(0), pVertices(0), pVertexBuffer(0), sizeInBytes(0)
{
	SetVertices(pVertices, count, vertexDeclaration);
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20VertexBuffer::~OGL20VertexBuffer(void)
{
	UnloadResource();
	UnloadVertices();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20VertexBuffer::SetVertices(void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration)
{
	UnloadVertices();

	SetVertexDeclaration(vertexDeclaration);

	int sizeInBytes = count * GetVertexSizeInBytes();

	this->pVertices = pVertices;

	this->sizeInBytes =  sizeInBytes;
	this->vertexDeclaration = vertexDeclaration;

	isDirty = true;
}
void mini3d::OGL20VertexBuffer::SetVertexDeclaration(const VertexDeclarationVector& vertexDeclaration)
{
	vertexSizeInBytes = 0;
	int count = vertexDeclaration.size();
	for (int i = 0; i < count; i++)
	{
		switch (vertexDeclaration[i])
		{
				
		case IVertexBuffer::POSITION:
			vertexSizeInBytes += 16;
			break;
		case IVertexBuffer::COLOR:
			vertexSizeInBytes += 16;
			break;
		case IVertexBuffer::TEXTURECOORDINATE:
			vertexSizeInBytes += 8;
			break;
		}
	}
}
unsigned int mini3d::OGL20VertexBuffer::GetVertexSizeInBytes(void)
{
	return vertexSizeInBytes;
}

void mini3d::OGL20VertexBuffer::UnloadVertices(void)
{
	if (pVertices != 0)
		operator delete(pVertices);

	pVertices = 0;
	sizeInBytes = 0;
}
void* mini3d::OGL20VertexBuffer::GetVertices(unsigned int& count, VertexDeclarationVector& vertexDeclaration)
{
	void* pReturnVertices = pVertices;
	count = GetVertexCount();
	vertexDeclaration = this->vertexDeclaration;
	
	// reset the data because we are "removing it" when we do a GetVertices!
	pVertices = 0;
	sizeInBytes = 0;
	vertexDeclaration = VertexDeclarationVector();

	isDirty = true;

	return pReturnVertices;
}
unsigned int mini3d::OGL20VertexBuffer::GetVertexCount()
{
	return this->sizeInBytes / GetVertexSizeInBytes();
}
mini3d::IVertexBuffer::VertexDeclarationVector mini3d::OGL20VertexBuffer::GetVertexDeclaration()
{
	return vertexDeclaration;
}
GLuint mini3d::OGL20VertexBuffer::GetVertexBuffer()
{
	return pVertexBuffer;
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

		try {
			glBufferData(GL_ARRAY_BUFFER, sizeInBytes, pVertices, GL_STATIC_DRAW);
		}
		catch (GLuint error)
		{
			//GL_OUT_OF_MEMORY
			isDirty = true;
			return;
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
bool mini3d::OGL20VertexBuffer::GetIsDirty(void)
{
	return isDirty;
}
