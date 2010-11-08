
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9GRAPHICSSETTINGS_H
#define MINI3D_D3D9GRAPHICSSETTINGS_H

#include "..\IWindowRenderTarget.h"

namespace mini3d
{
class D3D9GraphicsSettings
{
public:
	IWindowRenderTarget* pDefaultSwapChain;
};
}

#endif