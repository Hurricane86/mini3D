
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_DX9BITMAPTEXTURE_H
#define MINI3D_DX9BITMAPTEXTURE_H

#include "../IBitmapTexture.h"
#include "internal/IDX9Texture.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9Resource.h"


namespace mini3d
{
class DX9BitmapTexture : public IBitmapTexture, public IDX9Texture, public IDX9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	DX9BitmapTexture(DX9GraphicsService* graphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	~DX9BitmapTexture();


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

	virtual void LoadResource();
	virtual void UnloadResource();
	virtual bool GetIsDirty() const { return isDirty; };


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::
	virtual IDirect3DTexture9* GetTextureBuffer() const { return pTexture; };


private:

	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	void UnloadBitmap();
	unsigned int GetBytesPerPixel();


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
