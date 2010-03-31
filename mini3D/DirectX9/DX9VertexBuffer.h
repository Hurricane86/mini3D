
#ifndef AURORA_DX9VERTEXBUFFER_H
#define AURORA_DX9VERTEXBUFFER_H

#include "../IVertexBuffer.h"
#include "DX9GraphicsService.h"
#include "internal/IDX9Resource.h"

class DX9VertexBuffer :	public IVertexBuffer, public IDX9Resource
{
friend class DX9GraphicsService;

private:
	// vertices
	void* pVertices;
	unsigned int sizeInBytes;
	unsigned int vertexSizeInBytes;
	VertexDeclarationVector vertexDeclaration;
	
	// Buffer
	IDirect3DVertexBuffer9* pVertexBuffer;
	IDirect3DVertexDeclaration9* pVertexDeclaration;
	int bufferSizeInBytes;
	bool isDirty;

	// GraphicsDevice link
	DX9GraphicsService* pGraphicsService;

private:
	void UnloadVertices(void);
	IDirect3DVertexBuffer9* GetVertexBuffer();
	IDirect3DVertexDeclaration9* GetVertexDeclarationBuffer();
	void SetVertexDeclaration(const VertexDeclarationVector& vertexDeclaration);
	unsigned int GetVertexSizeInBytes(void);

public:
	DX9VertexBuffer(void);
	DX9VertexBuffer(DX9GraphicsService* pGraphicsService, void* pVertices, unsigned int count, const VertexDeclarationVector& vertexDeclaration);
	~DX9VertexBuffer(void);

	virtual void SetVertices(void* vertices, unsigned int count,const VertexDeclarationVector& vertexDeclaration);
	virtual void* GetVertices(unsigned int& count, VertexDeclarationVector& vertexDeclaration);
	
	unsigned int GetVertexCount(void);
	virtual VertexDeclarationVector GetVertexDeclaration();

	// IResource
	virtual void LoadResource(void);
	virtual void UnloadResource(void);
	virtual bool GetIsDirty(void);
};

#endif
