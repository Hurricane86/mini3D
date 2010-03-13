
#ifndef AURORA_IRENDERTARGET_H
#define AURORA_IRENDERTARGET_H

class IRenderTarget
{
public:
	virtual void SetRenderTarget(unsigned int width, unsigned int height) = 0;
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;
};

#endif
