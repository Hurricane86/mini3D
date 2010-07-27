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
3
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef AURORA_ISCREENRENDERTARGET_H
#define AURORA_ISCREENRENDERTARGET_H

#include "IRenderTarget.h"

namespace mini3d
{
class IScreenRenderTarget : public virtual IRenderTarget
{
public:

	enum Quality{QUALITY_MINIMUM, QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH, QUALITY_MAXIMUM};

	virtual ~IScreenRenderTarget() {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual bool GetDepthTestEnabled() const = 0;
	virtual Quality GetQuality() const = 0;
	virtual int GetWindowHandle() const = 0;
	
	virtual void Display() = 0;
};
}

#endif