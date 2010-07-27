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

#ifndef AURORA_OGL20BITMAPTEXTURE_H
#define AURORA_OGL20BITMAPTEXTURE_H

#include "../IBitmapTexture.h"
#include "internal/IOGL20Texture.h"
#include "OGL20GraphicsService.h"
#include "internal/IOGL20Resource.h"
#include <Windows.h>
#include <GL/gl.h>

namespace mini3d
{
class OGL20BitmapTexture : public IBitmapTexture, public IOGL20Texture, public IOGL20Resource
{
friend class OGL20GraphicsService;

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	OGL20BitmapTexture(OGL20GraphicsService* graphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	~OGL20BitmapTexture();

	// ::::: IBitmapTexture :::::::::::::::::::::::::::::::::::::::::::::::::::
	
	virtual void* GetBitmap(unsigned int& sizeInBytes) const;
	virtual void SetBitmap(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);

	virtual void* Lock(unsigned int& sizeInBytes) const;
	virtual void Unlock(const bool& dataIsChanged);

	inline unsigned int GetWidth() const { return width; };
	inline unsigned int GetHeight() const { return height; };

	inline WrapStyle GetWrapStyle() const { return wrapStyle; };
	virtual void SetWrapStyle(const WrapStyle& wrapStyle) { this->wrapStyle = wrapStyle; };

	inline BitDepth GetBitDepth() const { return bitDepth; };


	// ::::: IOGL20Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };

	
	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual GLuint GetTextureBuffer() const { return pTexture; };


private:

	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::
	
	void UnloadBitmap();
	unsigned int GetBytesPerPixel();

private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::

	// GraphicsDevice link
	OGL20GraphicsService* pGraphicsService;


	// Pointer to the bitmap data
	void* pBitmap;

	// Size of the bitmap
	unsigned int width;
	unsigned int height;
	
	// Size of the bitmap buffer in bytes
	unsigned int sizeInBytes;

	// wrapstyle
	ITexture::WrapStyle wrapStyle;

	//Bitdepth
	IBitmapTexture::BitDepth bitDepth;
	
	// Buffer
	GLuint pTexture;
	
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	
	// Keeps track of the state of the resource
	bool isDirty;


};
}

#endif
