
#ifndef AURORA_IVERTEXBUFFER_H
#define AURORA_IVERTEXBUFFER_H

#include "VertexDeclaration.h"

class IVertexBuffer
{
public:
	virtual void SetVertices(void* pVertices, unsigned int count, const VertexDeclaration& vertexDeclaration) = 0;
	virtual void* GetVertices(unsigned int& count, VertexDeclaration& vertexDeclaration) = 0;

	virtual unsigned int GetVertexCount() = 0;
	virtual VertexDeclaration GetVertexDeclaration() = 0;
};

#endif