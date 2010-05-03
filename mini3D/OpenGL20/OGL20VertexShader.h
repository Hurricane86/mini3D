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
#include <vector>
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
	friend class OGL20ShaderProgram;

private:
	// Indices
	ShaderBytes shaderBytes_;
	VertexDeclarationVector vertexDeclaration_;

	typedef std::vector<OGL20VertexAttribute> VertexAttributeVector;
	VertexAttributeVector vertexAttributes;
	
	// Buffer
	GLuint pShaderBuffer_;
	bool isDirty_;

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService_;

public:
	OGL20VertexShader(OGL20GraphicsService* pGraphicsService, const ShaderBytes& shaderBytes, const VertexDeclarationVector& vertexDeclaration);
	~OGL20VertexShader(void);

	virtual ShaderBytes GetVertexShader(void) { return shaderBytes_; };
	virtual VertexDeclarationVector GetVertexDeclaration(void) { return vertexDeclaration_; };
	VertexAttributeVector GetVertexAttributes(void) { return vertexAttributes; };

private:
	void CreateOGL20VertexAttributes();
	virtual GLuint GetVertexShaderBuffer(void) { return pShaderBuffer_; };
	
	void printLog(GLuint obj);
	// IOGL20Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void) { return isDirty_; };
};
}

#endif
