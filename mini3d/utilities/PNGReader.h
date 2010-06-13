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

#ifndef MINI3D_PNGREADER_H
#define MINI3D_PNGREADER_H

#include <Windows.h>
#include <GdiPlus.h>
#include <GdiPlusBitmap.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "BinaryFileReader.h"

namespace mini3d
{
namespace utilites
{
class PNGReader
{
public:
	 
	static void* LoadPNGFromFile(WCHAR* fileName, unsigned int& width, unsigned int& height)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,0);

		Gdiplus::Bitmap* b = Gdiplus::Bitmap::FromFile(fileName);

		Gdiplus::BitmapData bitmapData;
		b->LockBits(0, 0, PixelFormat32bppARGB, &bitmapData);

		width = bitmapData.Width;
		height = bitmapData.Height;

		void* pBitmapData = malloc(width * height * 4);
		for(unsigned int row = 0; row < height; ++row)
		{
			memcpy((char*)pBitmapData + (row * width * 4), (char*)(bitmapData.Scan0) + (row * bitmapData.Stride), width * 4);
		}
		
		b->UnlockBits(&bitmapData);
		delete b;

		Gdiplus::GdiplusShutdown(gdiplusToken);

		return pBitmapData;

	}

};
}
}

#endif
