
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9RENDERTARGET_H
#define MINI3D_DX9RENDERTARGET_H

#include "../../internal/IRenderTarget.h"

namespace mini3d
{
class DX9DepthStencil;

class IDX9RenderTarget: public virtual IRenderTarget
{


public:
	virtual ~IDX9RenderTarget(void) {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual bool GetDepthTestEnabled() const = 0;
	virtual bool GetFullscreenCompatible() const = 0;
	virtual bool GetWindowedCompatible() const = 0;

	virtual DX9DepthStencil* GetDepthStencil() const = 0;

	virtual IDirect3DSurface9* GetRenderTargetBuffer() const = 0;
};
}

#endif
