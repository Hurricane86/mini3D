/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../OGL20BitmapTexture.h"
#include <GL/glext.h>
#include <GL/wglext.h>


mini3d::OGL20BitmapTexture::OGL20BitmapTexture(OGL20GraphicsService* pGraphicsService, void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth, ITexture::WrapStyle wrapStyle) :
	pGraphicsService(pGraphicsService), bufferWidth(0), bufferHeight(0), pBitmap(0), pTexture(0)
{
	SetBitmap(pBitmap, width, height);
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::OGL20BitmapTexture::~OGL20BitmapTexture(void)
{
	UnloadResource();
	UnloadBitmap();
	pGraphicsService->RemoveResource(this);
}
GLuint mini3d::OGL20BitmapTexture::GetTextureBuffer(void)
{
	return pTexture;
}
void* mini3d::OGL20BitmapTexture::GetBitmap(unsigned int& width, unsigned int& height, IBitmapTexture::BitDepth& bitDepth, ITexture::WrapStyle& wrapStyle)
{
	void* pReturnBitmap = pBitmap;
	width = this->width;
	height = this->height;
	wrapStyle = this->wrapStyle;
	bitDepth = this->bitDepth;
	

	// reset the bitmap information because we are "removing" the bitmap from the texture when we get it!
	this->width = 0;
	this->height = 0;
	this->pBitmap = 0;

	wrapStyle = IBitmapTexture::TILE;
	bitDepth = IBitmapTexture::BIT32;

	this->isDirty = true;

	return pReturnBitmap;
}
void mini3d::OGL20BitmapTexture::SetBitmap(void* pBitmap, unsigned int width, unsigned int height, IBitmapTexture::BitDepth bitDepth, ITexture::WrapStyle wrapStyle)
{
	UnloadBitmap();
	
	int sizeInBytes = width * height * 4;  // TODO: Depends on graphics settings
	this->pBitmap = pBitmap;

	this->width = width;
	this->height = height;
	
	this->wrapStyle = wrapStyle;
	this->bitDepth = bitDepth;

	isDirty = true;
}
void mini3d::OGL20BitmapTexture::UnloadBitmap(void)
{
	if (pBitmap != 0)
		operator delete(pBitmap);

	pBitmap = 0;
	width = 0;
	height = 0;
	wrapStyle = IBitmapTexture::TILE;
	bitDepth = IBitmapTexture::BIT32;
}
unsigned int mini3d::OGL20BitmapTexture::GetWidth(void)
{
	return width;
}
unsigned int mini3d::OGL20BitmapTexture::GetHeight(void)
{
	return height;
}
mini3d::ITexture::WrapStyle mini3d::OGL20BitmapTexture::GetWrapStyle(void)
{
	return wrapStyle;
}
mini3d::IBitmapTexture::BitDepth mini3d::OGL20BitmapTexture::GetBitDepth(void)
{
	return bitDepth;
}
void mini3d::OGL20BitmapTexture::LoadResource(void)
{
	int sizeInBytes = width * height * 4;

    if (pBitmap == 0 || sizeInBytes == 0)
    {
		UnloadResource();
        return;
    }

	// If the buffer exists but is not the correct size, tear it down and recreate it
	if (pTexture != 0 && (bufferWidth != width || bufferHeight != height))
	{
		UnloadResource();
	}

	GLuint fmt;

	switch (bitDepth)
	{
	case IBitmapTexture::BIT16:
		fmt = GL_RGBA4;
		break;
	case IBitmapTexture::BIT32:
		fmt = GL_RGBA8;
		break;
	case IBitmapTexture::BIT64:
		fmt = GL_RGBA16;
		break;
	default:
		fmt = GL_RGBA8;
	}

	// If it does not exist, create a new one
	if (pTexture == 0)
	{
		glGenTextures(1, &pTexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, pTexture);
		
		try{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pBitmap);
		}
		catch  (GLuint error) //GL_OUT_OF_MEMORY
		{
			isDirty = true;
			return;			
		}
	}

	// write bitmap data to texture
	//glTexSubImage2D(pTexture, 0, 0, 0, width, height, fmt, GL_UNSIGNED_BYTE, pBitmap);

	// Clear the current bound texture
	glBindTexture(GL_TEXTURE_2D, 0);

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void mini3d::OGL20BitmapTexture::UnloadResource(void)
{
	if (pTexture != 0)
	{
		// if we are removing one of the current textures, clear that texture slot first
		for(int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		glDeleteTextures(1, &pTexture);
		pTexture = 0;
	}

	isDirty = true;
}
bool mini3d::OGL20BitmapTexture::GetIsDirty(void)
{
	return isDirty;
}
