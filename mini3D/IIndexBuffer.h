
#ifndef AURORA_IINDEXBUFFER_H
#define AURORA_IINDEXBUFFER_H

class IIndexBuffer
{
public:

	virtual ~IIndexBuffer(void) {};
	
	virtual void* GetIndices(unsigned int& sizeInBytes) = 0;
	virtual void SetIndices(void* pIndices, unsigned int count) = 0;

	virtual unsigned int GetIndexCount(void) = 0;
};

#endif