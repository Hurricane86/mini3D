
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifndef MINI3D_IOSWRAPPER_H
#define MINI3D_IOSWRAPPER_H

// Set the data type for the MINI3D_WINDOW macro
#include "../mini3d/datatypes/DataTypes.h"

namespace mini3d
{
class IOSWrapper
{
public:
	
	// Destructor
	virtual ~IOSWrapper() {};

	// Window States
	virtual void GetWindowContentSize(const MINI3D_WINDOW window, unsigned int &width, unsigned int &height) const = 0;
	virtual void SetFullscreenWindow(MINI3D_WINDOW window, const unsigned int& width, const unsigned int& height) = 0;
	virtual void RestoreFullscreenWindow(MINI3D_WINDOW window) = 0;
	virtual void Log(const char* message) const = 0;

};
}

#endif
