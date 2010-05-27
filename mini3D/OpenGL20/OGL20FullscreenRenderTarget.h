/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef AURORA_IOGL20FULLSCREENRENDERTARGET_H
#define AURORA_IOGL20FULLSCREENRENDERTARGET_H

#include "../IFullScreenRenderTarget.h"
#include "internal/IOGL20Resource.h"
#include "internal/IOGL20RenderTarget.h"

class OGL20FullscreenRenderTarget : public IFullscreenRenderTarget, public IOGL20RenderTarget, public IOGL20Resource
{
private:
	int width;
	int height;
	int hWindow;
	HDC hDeviceContext;
	bool depthTestEnabled;
	Quality quality;

public:
	enum Quality{QUALITY_MINIMUM, QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH, QUALITY_MAXIMUM};

	virtual ~OGL20FullscreenRenderTarget(void);

	virtual void SetScreenRenderTarget(unsigned int width, unsigned int height, int hWindow, bool depthTestEnabled, Quality quality){};
	virtual unsigned int GetWidth(void){ return width;};
	virtual unsigned int GetHeight(void){ return height;};
	virtual bool GetDepthTestEnabled(void){ return depthTestEnabled; };

	virtual void Display(void) {};
};

#endif