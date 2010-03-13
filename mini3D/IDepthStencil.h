
#ifndef AURORA_IDEPTHSTENCIL_H
#define AURORA_IDEPTHSTENCIL_H

class IDepthStencil
{
public:
	virtual void SetDepthStencil(unsigned int width, unsigned int height) = 0;
	virtual unsigned int GetWidth(void) = 0;
	virtual unsigned int GetHeight(void) = 0;
};

#endif