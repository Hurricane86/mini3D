
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DATATYPES_H
#define MINI3D_DATATYPES_H

// Set the data type for the MINI3D_WINDOW macro
#ifdef _WIN32
#ifndef MINI3D_WINDOW
#include <windows.h>
#define MINI3D_WINDOW HWND
#endif
#endif

#ifdef __linux
#ifndef MINI3D_WINDOW
#include <X11/Xlib.h>
#define MINI3D_WINDOW Window
#endif
#endif

#endif

