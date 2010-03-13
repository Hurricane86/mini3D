
#ifndef AURORA_IRENDERTARGETBUFFER_H
#define AURORA_IRENDERTARGETBUFFER_H

class IRenderTargetBuffer
{
public:
	virtual void SetRenderTargetBuffer(unsigned int width, unsigned int height, int hWindow) = 0;
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;
};

#endif
