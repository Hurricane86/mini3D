
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IDX9RESOURCE_H
#define MINI3D_IDX9RESOURCE_H

namespace mini3d
{
class IDX9Resource
{
public:

	virtual ~IDX9Resource(void) {};

	virtual void LoadResource() = 0;
	virtual void UnloadResource() = 0;
	virtual bool GetIsDirty() const = 0;
};
}

#endif
