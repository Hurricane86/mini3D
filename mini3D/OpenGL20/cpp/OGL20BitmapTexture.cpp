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
#include "../../error/error.h"
#include <GL/glext.h>
#include <GL/wglext.h>


mini3d::OGL20BitmapTexture::OGL20BitmapTexture(OGL20GraphicsService* pGraphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle) :
	pGraphicsService(pGraphicsService), bufferWidth(0), bufferHeight(0), pBitmap(0), pTexture(0)
{
	SetBitmap(pBitmap, width, height);
	pGraphicsService->AddResource(this);
}

mini3d::OGL20BitmapTexture::~OGL20BitmapTexture(void)
{
	UnloadResource();
	UnloadBitmap();
	pGraphicsService->RemoveResource(this);
}

void* mini3d::OGL20BitmapTexture::GetBitmap(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pBitmapCopy = malloc(sizeInBytes);
	memcpy(pBitmapCopy, pBitmap, sizeInBytes);
	
	return pBitmapCopy;
}
void mini3d::OGL20BitmapTexture::SetBitmap(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle)
{
	// If width or height is not a power of two
	if (width & (width - 1) != 0 || height & (height - 1))
	{
		throw Error::MINI3D_ERROR_NON_POWER_OF_TWO;
	}

	UnloadBitmap();

	this->bitDepth = bitDepth;
	sizeInBytes = width * height * GetBytesPerPixel();
	this->pBitmap = malloc(sizeInBytes);
	memcpy(this->pBitmap, pBitmap, sizeInBytes);

	this->width = width;
	this->height = height;
	
	this->wrapStyle = wrapStyle;

	isDirty = true;
	LoadResource();

}
void mini3d::OGL20BitmapTexture::UnloadBitmap(void)
{
	if (pBitmap != 0)
	{
		free(pBitmap);

		pBitmap = 0;
		width = 0;
		height = 0;
		sizeInBytes = 0;
		wrapStyle = IBitmapTexture::WRAP_TILE;
		bitDepth = IBitmapTexture::BIT_32;
	}
}

void* mini3d::OGL20BitmapTexture::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pBitmap;
}
void mini3d::OGL20BitmapTexture::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
}
void mini3d::OGL20BitmapTexture::LoadResource(void)
{
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
	case IBitmapTexture::BIT_16:
		fmt = GL_RGBA4;
		break;
	case IBitmapTexture::BIT_32:
		fmt = GL_RGBA8;
		break;
	case IBitmapTexture::BIT_64:
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
	//TODO: This instead of doing it above so we can recreate the texture...
	//glTexSubImage2D(pTexture, 0, 0, 0, width, height, fmt, GL_UNSIGNED_INT_8_8_8_8, pBitmap);

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

unsigned int mini3d::OGL20BitmapTexture::GetBytesPerPixel(void)
{
	switch(bitDepth)
	{
		case IBitmapTexture::BIT_16:
		return 2;
		case IBitmapTexture::BIT_32:
		return 4;
		case IBitmapTexture::BIT_64:
		return 8;
	}
}
