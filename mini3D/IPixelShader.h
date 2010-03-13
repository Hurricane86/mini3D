
#ifndef AURORA_IPIXELSHADER_H
#define AURORA_IPIXELSHADER_H

class IPixelShader
{
	friend class IGraphicsDevice;

public:
	virtual ~IPixelShader(void) {};

	virtual void* GetPixelShader(unsigned int& sizeInBytes) = 0;
	virtual void SetPixelShader(void* pShaderBytes, unsigned int sizeInBytes) = 0;
	virtual unsigned int GetSizeInBytes(void) = 0;

};

#endif