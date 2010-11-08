
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9TEXTURE_H
#define MINI3D_D3D9TEXTURE_H

#include "../../internal/ITexture.h"
#include <d3d9.h>

namespace mini3d
{
class ID3D9Texture: public virtual ITexture
{
friend class D3D9GraphicsService;

private:
	virtual IDirect3DTexture9* GetTextureBuffer() const = 0;

public:

	virtual ~ID3D9Texture(void) {};

	virtual unsigned int GetWidth() const= 0;
	virtual unsigned int GetHeight() const = 0;

	virtual WrapStyle GetWrapStyle() const = 0;
};
}

#endif
