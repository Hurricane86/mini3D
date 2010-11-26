
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20PIXELSHADER_H
#define MINI3D_OGL20PIXELSHADER_H

#include "../IPixelShader.h"
#include "internal/IOGL20Resource.h"
#include "oglwrapper/OGLWrapper.h"

namespace mini3d
{
class OGL20GraphicsService;
class OGL20PixelShader : public IPixelShader, IOGL20Resource
{

public:
	
	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::
	
	OGL20PixelShader(OGL20GraphicsService* graphicsService, const void* pShaderBytes, const unsigned int& sizeInBytes);
	~OGL20PixelShader(void);


	// ::::: IPixelShader :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void* GetPixelShader(unsigned int& sizeInBytes) { sizeInBytes = this->sizeInBytes; return pShaderBytes; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void) const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	inline GLuint GetPixelShaderBuffer(void) { return pShaderBuffer; }
	void printLog(GLuint obj);


private:

	
	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;
	OGLWrapper* pOGLWrapper;
	OSWrapper* pOSWrapper;

	// Indices
	void* pShaderBytes;

	// Number of bytes in shaderBytes array
	unsigned int sizeInBytes;

	// Buffer
	GLuint pShaderBuffer;

	// Keps track of the state of the resource
	bool isDirty;

};
}

#endif
