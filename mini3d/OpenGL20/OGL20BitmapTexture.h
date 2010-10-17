
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OGL20BITMAPTEXTURE_H
#define MINI3D_OGL20BITMAPTEXTURE_H

#include "../IBitmapTexture.h"
#include "internal/IOGL20Texture.h"
#include "internal/IOGL20Resource.h"

namespace mini3d
{
class OGL20GraphicsService;

class OGL20BitmapTexture : public IBitmapTexture, public IOGL20Texture, public IOGL20Resource
{

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
