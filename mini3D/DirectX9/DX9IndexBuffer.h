
#ifndef AURORA_DX9INDEXBUFFER_H
#define AURORA_DX9INDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "DX9GraphicsService.h"
#include "IDX9Resource.h"

class DX9IndexBuffer : IIndexBuffer, IDX9Resource
{

	friend class DX9GraphicsService;

private:
	// Indices
	void* pIndices;
	int sizeInBytes;
	
	// Buffer
	IDirect3DIndexBuffer9* pIndexBuffer;
	int bufferSizeInBytes;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	virtual IDirect3DIndexBuffer9* GetIndexBuffer(void);
	void UnloadIndices(void);

public:
	DX9IndexBuffer(DX9GraphicsService* graphicsService, void* pIndices, unsigned int count);
	~DX9IndexBuffer(void);

	virtual void SetIndices(void* pIndices, unsigned int count);
	virtual void* GetIndices(unsigned int& count);
	virtual unsigned int GetIndexCount(void);
	

	// IDX9Resource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);
};

#endif
