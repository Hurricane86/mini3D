
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_ERROR_H
#define MINI3D_ERROR_H

namespace mini3d
{
class Error
{
public:
	static const unsigned int MINI3D_ERROR_UNKNOWN = 0;
	static const unsigned int MINI3D_ERROR_NON_POWER_OF_TWO = 1;
	static const unsigned int MINI3D_ERROR_TEXTURE_INDEX_OUTSIDE_VALID_RANGE = 2;
	static const unsigned int MINI3D_ERROR_UNKNOWN_BACK_BUFFER_FORMAT = 3;
	static const unsigned int MINI3D_ERROR_NO_SUPPORTED_DEPTH_STENCIL_FORMAT = 4;
};
}

#endif