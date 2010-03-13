#pragma once

#include "../IVertexShader.h"
#include "DX9GraphicsService.h"

class DX9VertexShader : IVertexShader, IDX9Resource
{
friend class DX9GraphicsService;

private:
	// Indices
	void* pShaderBytes;
	unsigned int sizeInBytes;
	
	// Buffer
	IDirect3DVertexShader9* pShaderBuffer;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

public:
	DX9VertexShader(DX9GraphicsService* graphicsService, void* pShaderBytes, unsigned int sizeInBytes);
	~DX9VertexShader(void);

	virtual void SetVertexShader(void* pShaderBytes, unsigned int sizeInBytes);
	virtual void* GetVertexShader(unsigned int& sizeInBytes);
	

	virtual unsigned int GetSizeInBytes();

private:
	virtual IDirect3DVertexShader9* GetVertexShaderBuffer(void);
	void UnloadVertexShader(void);

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);
};
