
#ifndef AURORA_ITEXTURE_H
#define AURORA_ITEXTURE_H

class ITexture
{
public:

	enum WrapStyle
	{
		TILE,
		MIRROR,
		CLAMP,
	};

	enum BitDepth
	{
		BIT16,
		BIT32,
		BIT64
	};

	virtual ~ITexture(void) {};

	virtual void* GetBitmap(unsigned int& width, unsigned int& height, ITexture::WrapStyle& wrapStyle, ITexture::BitDepth& bitDepth) = 0;
	virtual void SetBitmap(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle = ITexture::TILE, ITexture::BitDepth bitDepth = ITexture::BIT32) = 0;
	
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;

	virtual WrapStyle GetWrapStyle(void) = 0;
	virtual BitDepth GetBitDepth(void) = 0;
};

#endif
