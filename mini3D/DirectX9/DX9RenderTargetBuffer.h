
#ifndef AURORA_DX9RENDERTARGETBUFFER_H
#define AURORA_DX9RENDERTARGETBUFFER_H

#include "../IRenderTargetBuffer.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9RenderTargetBuffer : public IRenderTargetBuffer, public IDX9Resource
{
friend class DX9GraphicsService;

private:
	// Bitmap
	unsigned int width;
	unsigned int height;
	int hWindow;
	bool isDirty;

	// Buffer
	unsigned int bufferWidth;
	unsigned int bufferHeight;
	int hBufferWindow;
	IDirect3DSwapChain9* pRenderTargetBuffer;
	
	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	virtual IDirect3DSurface9* GetBackBuffer(void);

public:
	DX9RenderTargetBuffer(DX9GraphicsService* graphicsService, unsigned int width, unsigned int height, int hWindow);
	~DX9RenderTargetBuffer(void);

	void SetRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow);
	virtual unsigned int GetWidth(void);
	virtual unsigned int GetHeight(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);

};

#endif
