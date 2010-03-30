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

#include "DX9BitmapTexture.h"
#include <d3d9.h>

DX9BitmapTexture::DX9BitmapTexture(DX9GraphicsService* pGraphicsService, void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, IBitmapTexture::BitDepth bitDepth) :
	pGraphicsService(pGraphicsService), bufferWidth(0), bufferHeight(0), pBitmap(0), pTexture(0)
{
	SetBitmap(pBitmap, width, height);
	LoadResource();
}

DX9BitmapTexture::~DX9BitmapTexture(void)
{
	UnloadResource();
	UnloadBitmap();
	pGraphicsService->RemoveResource(this);
}
IDirect3DTexture9* DX9BitmapTexture::GetTextureBuffer(void)
{
	return pTexture;
}
void* DX9BitmapTexture::GetBitmap(unsigned int& width, unsigned int& height, ITexture::WrapStyle& wrapStyle, IBitmapTexture::BitDepth& bitDepth)
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
void DX9BitmapTexture::SetBitmap(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, IBitmapTexture::BitDepth bitDepth)
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
void DX9BitmapTexture::UnloadBitmap(void)
{
	if (pBitmap != 0)
		operator delete(pBitmap);

	pBitmap = 0;
	width = 0;
	height = 0;
	wrapStyle = IBitmapTexture::TILE;
	bitDepth = IBitmapTexture::BIT32;
}
unsigned int DX9BitmapTexture::GetWidth(void)
{
	return width;
}
unsigned int DX9BitmapTexture::GetHeight(void)
{
	return height;
}
ITexture::WrapStyle DX9BitmapTexture::GetWrapStyle(void)
{
	return wrapStyle;
}
IBitmapTexture::BitDepth DX9BitmapTexture::GetBitDepth(void)
{
	return bitDepth;
}
void DX9BitmapTexture::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

	int sizeInBytes = width * height * 4;  // TODO: Depends on graphics settings

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

	// If it does not exist, create a new one
	if (pTexture == 0)
	{
		D3DFORMAT fmt;

		switch (bitDepth)
		{
		case IBitmapTexture::BIT16:
			fmt = D3DFMT_X4R4G4B4;
			break;
		case IBitmapTexture::BIT32:
			fmt = D3DFMT_X8R8G8B8;
			break;
		case IBitmapTexture::BIT64:
			fmt = D3DFMT_A16B16G16R16;
			break;
		default:
			fmt = D3DFMT_X8R8G8B8;
		}


		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &pTexture, 0 ) ) ) 
		{
			isDirty = true;
			return;
		}
	}

	// Lock the buffer to gain access to the Bitmap 
	IDirect3DSurface9* pSurface;
	pTexture->GetSurfaceLevel(0, &pSurface);
	
	D3DLOCKED_RECT textureDataRectangle;

	if(FAILED(pSurface->LockRect(&textureDataRectangle, 0, D3DLOCK_DISCARD)))
	{
		isDirty = true;
		pSurface->Release();
		return;
	}

	memcpy(textureDataRectangle.pBits, pBitmap, sizeInBytes);
	pSurface->UnlockRect();
	pSurface->Release();

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void DX9BitmapTexture::UnloadResource(void)
{
	if (pTexture != 0)
	{
		pTexture->Release();
		pTexture = 0;
	}

	isDirty = true;
}
bool DX9BitmapTexture::GetIsDirty(void)
{
	return isDirty;
}
