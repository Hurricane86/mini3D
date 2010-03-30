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

#ifndef AURORA_IBITMAPTEXTURE_H
#define AURORA_IBITMAPTEXTURE_H

#include "internal/ITexture.h"

class IBitmapTexture : public virtual ITexture
{
public:

	enum BitDepth
	{
		BIT16,
		BIT32,
		BIT64
	};

	virtual ~IBitmapTexture(void) {};

	virtual void* GetBitmap(unsigned int& width, unsigned int& height, ITexture::WrapStyle& wrapStyle, IBitmapTexture::BitDepth& bitDepth) = 0;
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle = ITexture::TILE, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32) = 0;
	
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;

	virtual WrapStyle GetWrapStyle(void) = 0;
	virtual BitDepth GetBitDepth(void) = 0;
};

#endif
