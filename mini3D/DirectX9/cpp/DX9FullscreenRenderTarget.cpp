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

#include "../DX9FullscreenRenderTarget.h"

mini3d::DX9FullscreenRenderTarget::DX9FullscreenRenderTarget(DX9GraphicsService* pGraphicsService, const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) 
	: pGraphicsService(pGraphicsService), width(width), height(height), quality(quality), depthTestEnabled(depthTestEnabled), hWindow(windowHandle)
{ 
	SetFullScreenRenderTarget(width, height, windowHandle, depthTestEnabled, quality);
};

mini3d::DX9FullscreenRenderTarget::~DX9FullscreenRenderTarget(void)
{
	// No need to restore anything here because if the fullscreenrendertarget is mounted it is the default render target already.
};

void mini3d::DX9FullscreenRenderTarget::SetFullScreenRenderTarget(const unsigned int& width, const unsigned int& height, const int& windowHandle, const bool& depthTestEnabled, const Quality& quality)
{ 
	this->width = width;
	this->height = height;
	this->hWindow = windowHandle;
	this->depthTestEnabled = depthTestEnabled;
	this->quality = quality;

	// if this is the mounted rendertarget, recreate the device to get a new default render target
	if (pGraphicsService->pCurrentRenderTarget == this)
	{
		pGraphicsService->RecreateDevice();
	}
};

void mini3d::DX9FullscreenRenderTarget::Display()
{
	// Make sure we do an endScene before we present (DirectX9 specific).
	if (pGraphicsService->isDrawingScene == true)
		pGraphicsService->EndScene();

	pGraphicsService->GetDevice()->Present(0,0,0,0);
}
