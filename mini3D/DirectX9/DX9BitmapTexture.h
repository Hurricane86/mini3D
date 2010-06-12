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
#include "internal/IDX9Texture.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9Resource.h"

// TODO: Power of two conversion for internal bitmaps

namespace mini3d
{
class DX9BitmapTexture : public IBitmapTexture, public IDX9Texture, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9BitmapTexture(DX9GraphicsService* graphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	~DX9BitmapTexture(void);


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


	// ::::: IDX9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void) const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::
	virtual IDirect3DTexture9* GetTextureBuffer(void) const { return pTexture; };


private:

	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadBitmap(void);


private:

	// ::::: Private Member Varaibles :::::::::::::::::::::::::::::::::::::::::
	
	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

	// Pointer to bitmap data
	void* pBitmap;

	// Size of the bitmap
	unsigned int width;
	unsigned int height;

	// Size of bitmap buffer in bytes
	unsigned int sizeInBytes;
	
	// WrapStyle
	ITexture::WrapStyle wrapStyle;

	// Bitdepth
	IBitmapTexture::BitDepth bitDepth;
	
	// Direct3D9 Buffer
	IDirect3DTexture9* pTexture;
	
	// Size of the currently loaded buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;

	// Keps track of the state of the resource
	bool isDirty;
};
}

#endif
