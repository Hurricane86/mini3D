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

#include "../OGL20RenderTargetTexture.h"
#include <GL/glext.h>
#include <GL/wglext.h>

mini3d::OGL20RenderTargetTexture::OGL20RenderTargetTexture(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) : 
	pGraphicsService(pGraphicsService), pDepthStencil(0)
{
	SetRenderTarget(width, height, depthTestEnabled);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20RenderTargetTexture::~OGL20RenderTargetTexture(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20RenderTargetTexture::SetRenderTarget(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	
	this->isDirty = true;
	LoadResource();
}

void mini3d::OGL20RenderTargetTexture::LoadResource(void)
{

	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");

	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");

	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");

	// if it is not unloaded, do so now
	UnloadResource();

	// create a texture object
	glGenTextures(1, &pTexture);
	glBindTexture(GL_TEXTURE_2D, pTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (depthTestEnabled == true)
	{
		// create a renderbuffer object to store depth info
		glGenRenderbuffers(1, &pDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, pDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	
	
	// create a framebuffer object
	glGenFramebuffers(1, &pRenderTarget);
	glBindFramebuffer(GL_FRAMEBUFFER, pRenderTarget);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture, 0);

	// attach the renderbuffer to depth attachment point
	if (depthTestEnabled == true)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pDepthStencil);
	}

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void mini3d::OGL20RenderTargetTexture::UnloadResource(void)
{
	if (pRenderTarget != 0)
	{
		
		PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
		PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");


		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		// if we are removing one of the current textures, clear that texture slot first
		for(int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		glDeleteRenderbuffers(1, &pDepthStencil);
		glDeleteTextures(1, &pTexture);
		glDeleteFramebuffers(1, &pRenderTarget);
		
		pDepthStencil = 0;
		pTexture = 0;
		pRenderTarget = 0;

	}

	isDirty = true;
}
