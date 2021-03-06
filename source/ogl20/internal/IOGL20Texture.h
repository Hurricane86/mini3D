
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20TEXTURE_H
#define MINI3D_OGL20TEXTURE_H

#include "../../mini3d/internal/ITexture.h"
#include "../oglwrapper/OGLWrapper.h"

namespace mini3d
{
class IOGL20Texture: public virtual ITexture
{

public:

	virtual ~IOGL20Texture() {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual WrapStyle GetWrapStyle() const = 0;

	virtual GLuint GetTextureBuffer() const = 0;

};
}

#endif
