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
