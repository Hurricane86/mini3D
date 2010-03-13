#pragma once

#include "../IPixelShader.h"
#include "DX9GraphicsService.h"

class DX9PixelShader : IPixelShader, IDX9Resource
{

	friend class DX9GraphicsService;

private:
	// Indices
	void* pShaderBytes;
	unsigned int sizeInBytes;
	
	// Buffer
	IDirect3DPixelShader9* pShaderBuffer;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

public:
	DX9PixelShader(DX9GraphicsService* graphicsService, void* pShaderBytes, unsigned int sizeInBytes);
	~DX9PixelShader(void);

	virtual void SetPixelShader(void* pShaderBytes, unsigned int sizeInBytes);
	virtual void* GetPixelShader(unsigned int& sizeInBytes);
	virtual unsigned int GetSizeInBytes();

private:

	virtual IDirect3DPixelShader9* GetPixelShaderBuffer(void);
	void UnloadPixelShader(void);

	// IDX9Resource
	void LoadResource(void);
	void UnloadResource(void);
	bool GetIsDirty(void);
};
