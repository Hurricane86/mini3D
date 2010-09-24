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

#ifndef AURORA_OGL20INDEXBUFFER_H
#define AURORA_OGL20INDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20Resource.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
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
