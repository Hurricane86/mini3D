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

#ifndef AURORA_OGL20RENDERTARGETTEXTURE_H
#define AURORA_OGL20RENDERTARGETTEXTURE_H

#include "../IRenderTargetTexture.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20RenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20Texture.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20RenderTargetTexture : public IRenderTargetTexture, public IOGL20Texture, public IOGL20RenderTarget, public IOGL20Resource
{
friend class OGL20GraphicsService;

private:
	// Bitmap
	unsigned int width;
	unsigned int height;
	bool depthTestEnabled;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	GLuint pTexture;
	GLuint pRenderTarget;
	GLuint pDepthStencil;
	
	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;

private:
	virtual GLuint GetRenderTargetBuffer(void);
	virtual GLuint GetTextureBuffer(void);
	virtual int GetType() { return 1; }

public:
	OGL20RenderTargetTexture(OGL20GraphicsService* graphicsService, unsigned int width, unsigned int height, bool depthTestEnabled);
	~OGL20RenderTargetTexture(void);

	void SetRenderTarget(unsigned int width, unsigned int height, bool depthTestEnabled);
	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);
	virtual bool GetDepthTestEnabled(void);
	virtual WrapStyle GetWrapStyle(void);

	// IOGL20Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

};
}

#endif
