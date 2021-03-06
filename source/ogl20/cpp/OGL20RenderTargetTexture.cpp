
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "../OGL20RenderTargetTexture.h"
#include "../OGL20GraphicsService.h"

mini3d::OGL20RenderTargetTexture::OGL20RenderTargetTexture(OGL20GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) : 
	pGraphicsService(pGraphicsService), pDepthStencil(0), pOGLWrapper(pGraphicsService->GetOGLWrapper())
{
	SetRenderTargetTexture(width, height, depthTestEnabled);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20RenderTargetTexture::~OGL20RenderTargetTexture(void)
{
	UnloadResource();
	pGraphicsService->RemoveResource(this);
}

void mini3d::OGL20RenderTargetTexture::SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled)
{
	this->width = width;
	this->height = height;
	this->depthTestEnabled = depthTestEnabled;
	
	this->isDirty = true;
	LoadResource();
}

void mini3d::OGL20RenderTargetTexture::LoadResource(void)
{

	// if it is not unloaded, do so now
	UnloadResource();

	// create a texture object
	glGenTextures(1, &pTexture);
	pOGLWrapper->GLBindTexture(GL_TEXTURE_2D, pTexture);
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
		pOGLWrapper->GLGenRenderbuffers(1, &pDepthStencil);
		pOGLWrapper->GLBindRenderbuffer(GL_RENDERBUFFER, pDepthStencil);
		pOGLWrapper->GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		pOGLWrapper->GLBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	
	
	// create a framebuffer object
	pOGLWrapper->GLGenFramebuffers(1, &pRenderTarget);
	pOGLWrapper->GLBindFramebuffer(GL_FRAMEBUFFER, pRenderTarget);

	// attach the texture to FBO color attachment point
	pOGLWrapper->GLFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture, 0);

	// attach the renderbuffer to depth attachment point
	if (depthTestEnabled == true)
	{
		pOGLWrapper->GLFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pDepthStencil);
	}

	// switch back to window-system-provided framebuffer
	pOGLWrapper->GLBindFramebuffer(GL_FRAMEBUFFER, 0);

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void mini3d::OGL20RenderTargetTexture::UnloadResource(void)
{
	if (pRenderTarget != 0)
	{
		// if we are removing the current render target, restore the default render target first
		if (pGraphicsService->GetRenderTarget() == this)
			pGraphicsService->SetRenderTarget(0);

		// if we are removing one of the current textures, clear that texture slot first
		for(unsigned int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		pOGLWrapper->GLDeleteRenderbuffers(1, &pDepthStencil);
		glDeleteTextures(1, &pTexture);
		pOGLWrapper->GLDeleteFramebuffers(1, &pRenderTarget);
		
		pDepthStencil = 0;
		pTexture = 0;
		pRenderTarget = 0;
	}

	isDirty = true;
}
