
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

public:
	DX9Texture(DX9GraphicsService* graphicsService, void* pBitmap, unsigned int width, unsigned int height);
	~DX9Texture(void);

	virtual void* GetBitmap(unsigned int& width, unsigned int& height);
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height);

	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);
};

#endif
