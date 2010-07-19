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

#ifndef AURORA_OGL20VERTEXSHADER_H
#define AURORA_OGL20VERTEXSHADER_H

#include "../IVertexShader.h"
#include "OGL20GraphicsService.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{

struct OGL20VertexAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLvoid* pointer;
};

class OGL20VertexShader : IVertexShader, IOGL20Resource
{
	friend class OGL20GraphicsService;

public:
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::


public:
	OGL20VertexShader(OGL20GraphicsService* pGraphicsService, const void* shaderBytes, const unsigned int& sizeInBytes, const VertexDataType vertexDeclaration[], const unsigned int& vertexDataCount);
	~OGL20VertexShader(void);

	virtual void* GetVertexShader(unsigned int& sizeInBytes) const { sizeInBytes = this->sizeInBytes; return pShaderBytes; }
	virtual VertexDataType* GetVertexDeclaration(unsigned int& vertexDataCount) const;
	
	
	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };

	
	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual GLuint GetVertexShaderBuffer(void) { return pShaderBuffer; };
	virtual OGL20VertexAttribute* GetVertexAttributes(unsigned int& vertexAttributeCount) { vertexAttributeCount = vertexDataCount; return vertexAttributes; };

private:

	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	void CreateOGL20VertexAttributes();
	void printLog(GLuint obj);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// Indices
	void* pShaderBytes;
	unsigned int sizeInBytes;

	VertexDataType* vertexDeclaration;
	unsigned int vertexDataCount;

	OGL20VertexAttribute* vertexAttributes;
	
	// Buffer
	GLuint pShaderBuffer;
	bool isDirty;

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;

};
}

#endif
