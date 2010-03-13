
#ifndef AURORA_IDX9RESOURCE_H
#define AURORA_IDX9RESOURCE_H

class IDX9Resource
{
public:

	virtual ~IDX9Resource(void) {};

	virtual void LoadResource(void) = 0;
	virtual void UnloadResource(void) = 0;
	virtual bool GetIsDirty(void) = 0;
};



#endif