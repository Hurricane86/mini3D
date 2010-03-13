
#ifndef AURORA_ITEXTURE_H
#define AURORA_ITEXTURE_H

class ITexture
{
public:

	virtual ~ITexture(void) {};

	virtual void* GetBitmap(unsigned int& width, unsigned int& height) = 0;
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height) = 0;
	
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;
};

#endif
