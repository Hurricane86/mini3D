
#ifndef AURORA_DX9DEPTHSTENCIL_H
#define AURORA_DX9DEPTHSTENCIL_H

#include "../IDepthStencil.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9DepthStencil : public IDepthStencil, public IDX9Resource
{
friend class DX9GraphicsService;

private:
	// Bitmap
	unsigned int width;
	unsigned int height;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	IDirect3DSurface9* pDepthStencil;
	
	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	virtual IDirect3DSurface9* GetDepthStencilBuffer(void);

public:
	DX9DepthStencil(DX9GraphicsService* graphicsService, unsigned int width, unsigned int height);
	~DX9DepthStencil(void);

	void SetDepthStencil(unsigned int width, unsigned int height);
	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

};

#endif
