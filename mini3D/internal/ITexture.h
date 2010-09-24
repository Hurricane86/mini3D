
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_ITEXTURE_H
#define AURORA_ITEXTURE_H

namespace mini3d
{
class ITexture
{
public:

	enum WrapStyle { WRAP_TILE, WRAP_MIRROR, WRAP_CLAMP, };

	virtual ~ITexture(void) {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual WrapStyle GetWrapStyle() const = 0;
};
}

#endif
