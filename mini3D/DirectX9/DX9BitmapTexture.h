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

#ifndef AURORA_DX9BITMAPTEXTURE_H
#define AURORA_DX9BITMAPTEXTURE_H

#include "../IBitmapTexture.h"
#include "IDX9Texture.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9BitmapTexture : public IBitmapTexture, public IDX9Texture, public IDX9Resource
{
friend class DX9GraphicsService;

public:
	// IBitmapTexture
	virtual void* GetBitmap(unsigned int& width, unsigned int& height, IBitmapTexture::BitDepth& bitDepth, ITexture::WrapStyle& wrapStyle);
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE);
	
	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);
	virtual WrapStyle GetWrapStyle(void);
	virtual BitDepth GetBitDepth(void);

	//Constructor
	DX9BitmapTexture(DX9GraphicsService* graphicsService, void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT32, ITexture::WrapStyle wrapStyle = ITexture::TILE);

	// Destructor
	~DX9BitmapTexture(void);



private:
	// Accessed by Graphics Service	
	virtual IDirect3DTexture9* GetTextureBuffer(void);
	
	// unload the bitmap data
	void UnloadBitmap(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

private:
	// Bitmap
	void* pBitmap;
	int width;
	int height;
	ITexture::WrapStyle wrapStyle;
	IBitmapTexture::BitDepth bitDepth;
	
	// Buffer
	IDirect3DTexture9* pTexture;
	
	// TODO: Fix this with power of 2 comparison agains width, height...
	int bufferWidth;
	int bufferHeight;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

};

#endif
