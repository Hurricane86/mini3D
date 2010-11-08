
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_D3D9BITMAPTEXTURE_H
#define MINI3D_D3D9BITMAPTEXTURE_H

#include "../IBitmapTexture.h"
#include "internal/ID3D9Texture.h"
#include "D3D9GraphicsService.h"
#include "internal/ID3D9Resource.h"


namespace mini3d
{
class D3D9BitmapTexture : public IBitmapTexture, public ID3D9Texture, public ID3D9Resource
{

public:

	// ::::: Constructor & Destructor :::::::::::::::::::::::::::::::::::::::::

	D3D9BitmapTexture(D3D9GraphicsService* graphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE);
	~D3D9BitmapTexture();


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


	// ::::: ID3D9Resource :::::::::::::::::::::::::::::::::::::::::::::::::::::

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
	D3D9GraphicsService* pGraphicsService;

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
