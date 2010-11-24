
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OSWRAPPER_H
#define MINI3D_OSWRAPPER_H

#ifdef _WIN32
	#include "windows/OSWrapperWindows.h"
#endif

#ifdef __linux
	#include "linux/OSWrapperLinux.h"
#endif

#endif