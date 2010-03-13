
#ifndef AURORA_VERTEXDECLARATION_H
#define AURORA_VERTEXDECLARATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

struct VertexDeclaration
{

public:

	enum VertexDataType
	{
		POSITION,
		COLOR,
		TEXTURECOORDINATE,
		NORMAL,
		BINORMAL,
		TANGENT,
		BLENDWEIGHT,
		BLENDINDICES
	};

public: VertexDeclaration& operator=(const VertexDeclaration &rhs)
	{
		sizeInBytes = rhs.sizeInBytes;
		
		if (vertexDataTypes != 0)
		{
			delete[] vertexDataTypes;
			vertexDataTypes = 0;
		}
		
		vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(&vertexDataTypes[0], &rhs.vertexDataTypes[0], sizeInBytes);

		return *this;
	}
	VertexDeclaration(const VertexDeclaration& vertexDeclaration) : vertexDataTypes(0)
	{
		sizeInBytes = vertexDeclaration.sizeInBytes;
		vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(&vertexDataTypes[0], &vertexDeclaration.vertexDataTypes[0], sizeInBytes);
	}
	VertexDeclaration(void) : vertexDataTypes(0)
	{
	}
	VertexDeclaration(const VertexDataType* vertexDataTypes, unsigned int sizeInBytes) :
		sizeInBytes(sizeInBytes), vertexDataTypes(0)
	{
		// copy the vertexdatatypes to a member pointer
		this->vertexDataTypes = new VertexDataType[sizeInBytes / sizeof(VertexDataType)];
		memcpy(this->vertexDataTypes, vertexDataTypes, sizeInBytes);
	};

	~VertexDeclaration(void)
	{
		if (vertexDataTypes != 0)
		{
			delete[] vertexDataTypes;
			vertexDataTypes = 0;
		}
	};

	VertexDataType* GetVertexDataTypes(unsigned int& sizeInBytes) const
	{
		sizeInBytes = this->sizeInBytes;
		return vertexDataTypes;
	};

	inline int GetSizeInBytes(void) const
	{
		return sizeInBytes;
	};

private:

	VertexDataType* vertexDataTypes;
	int sizeInBytes;
};



#endif