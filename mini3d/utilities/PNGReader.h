
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


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
		
		unsigned int size = width * height;

		unsigned int* pPixels = (unsigned int*)pBitmapData;

		//Flip the channel order to RGBA
		for (unsigned int i = 0; i < size; i++)
		{
			pPixels[i] = pPixels[i]<<8 | pPixels[i]>>24;
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
