
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20VERTEXSHADER_H
#define MINI3D_OGL20VERTEXSHADER_H

#include "../mini3d/IVertexShader.h"
#include "internal/IOGL20Resource.h"
#include "oglwrapper/OGLWrapper.h"

namespace mini3d
{
class OGL20GraphicsService;
class OGLWrapper;
class OSWrapper;

struct OGL20VertexAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLvoid* pointer;
};

class OGL20VertexShader : public IVertexShader, IOGL20Resource
{
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
	OGLWrapper* pOGLWrapper;
	OSWrapper* pOSWrapper;

};
}

#endif
