
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20SHADERPROGRAM_H
#define MINI3D_OGL20SHADERPROGRAM_H

#include "../mini3d/IPixelShader.h"
#include "../mini3d/IVertexShader.h"
#include "../mini3d/IShaderProgram.h"
#include "internal/IOGL20Resource.h"
#include "oglwrapper/OGLWrapper.h"

namespace mini3d
{
class OGL20GraphicsService;
class OGLWrapper;
class OSWrapper;

class OGL20ShaderProgram : public IShaderProgram, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20ShaderProgram(OGL20GraphicsService* graphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~OGL20ShaderProgram();


	// ::::: IShaderProgram :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IPixelShader* GetPixelShader() const { return pPixelShader; };
	virtual IVertexShader* GetVertexShader() const { return pVertexShader; };


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	void LoadResource();
	void UnloadResource();
	bool GetIsDirty() const { return isDirty; };


	// ::::: Public Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	inline GLuint GetProgram() { return hProgram; }
	void printLog(GLuint obj);


private:
	// Indices
	IPixelShader* pPixelShader;
	IVertexShader* pVertexShader;

	GLuint hProgram;

	bool isDirty;

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;
	OGLWrapper* pOGLWrapper;
	OSWrapper* pOSWrapper;

};
}

#endif
