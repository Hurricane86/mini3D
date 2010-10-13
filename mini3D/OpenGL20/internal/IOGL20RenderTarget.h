
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20RENDERTARGET_H
#define MINI3D_OGL20RENDERTARGET_H

#include "../../internal/IRenderTarget.h"

namespace mini3d
{
class IOGL20RenderTarget: public virtual IRenderTarget
{
friend class OGL20GraphicsService;

public:
	virtual ~IOGL20RenderTarget() {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual bool GetDepthTestEnabled() const = 0;
};
}

#endif
