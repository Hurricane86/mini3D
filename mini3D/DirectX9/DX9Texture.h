
#ifndef AURORA_DX9TEXTURE_H
#define AURORA_DX9TEXTURE_H

#include "../ITexture.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9Texture : public ITexture, public IDX9Resource
{
friend class DX9GraphicsService;

private:
	// Bitmap
	void* pBitmap;
	int width;
	int height;
	ITexture::WrapStyle wrapStyle;
	ITexture::BitDepth bitDepth;
	
	// Buffer
	IDirect3DTexture9* pTexture;
	
	// TODO: Fix this with power of 2 comparison agains width, height...
	int bufferWidth;
	int bufferHeight;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	virtual IDirect3DTexture9* GetTexture(void);
	void UnloadBitmap(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

public:
	DX9Texture(DX9GraphicsService* graphicsService, void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle = ITexture::TILE, ITexture::BitDepth bitDepth = ITexture::BIT32);
	~DX9Texture(void);

	virtual void* GetBitmap(unsigned int& width, unsigned int& height, ITexture::WrapStyle& wrapStyle, ITexture::BitDepth& bitDepth);
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle = ITexture::TILE, ITexture::BitDepth bitDepth = ITexture::BIT32);

	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);

	virtual WrapStyle GetWrapStyle(void);
	virtual BitDepth GetBitDepth(void);
};

#endif
