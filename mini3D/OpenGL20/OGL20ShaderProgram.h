
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_OGL20SHADERPROGRAM_H
#define AURORA_OGL20SHADERPROGRAM_H

#include "../IPixelShader.h"
#include "../IVertexShader.h"
#include "../IShaderProgram.h"
#include "internal/IOGL20Resource.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20GraphicsService;
class OGL20ShaderProgram : public IShaderProgram, public IOGL20Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20ShaderProgram(OGL20GraphicsService* graphicsService, IVertexShader* pVertexShader, IPixelShader* pPixelShader);
	~OGL20ShaderProgram();


	// ::::: IShaderProgram :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual IPixelShader* GetPixelShader() const { return pPixelShader; };
	virtual IVertexShader* GetVertexShader() const { return pVertexShader; };


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

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


};
}

#endif
