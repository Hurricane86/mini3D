
#ifndef AURORA_IVERTEXSHADER_H
#define AURORA_IVERTEXSHADER_H

class IVertexShader
{
public:
	virtual ~IVertexShader(void) {};

	virtual void* GetVertexShader(unsigned int& sizeInBytes) = 0;
	virtual void SetVertexShader(void* pShaderBytes, unsigned int sizeInBytes) = 0;
	virtual unsigned int GetSizeInBytes(void) = 0;
};

#endif