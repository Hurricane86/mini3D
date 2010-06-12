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

// TODO: Bitmap shall be recreate if call to SetBitmap with new bitDepth format

#include "../DX9BitmapTexture.h"
#include <d3d9.h>

mini3d::DX9BitmapTexture::DX9BitmapTexture(DX9GraphicsService* pGraphicsService, const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle) :
	pGraphicsService(pGraphicsService), bufferWidth(0), bufferHeight(0), pBitmap(0), pTexture(0)
{
	SetBitmap(pBitmap, width, height);
	pGraphicsService->AddResource(this);
}

mini3d::DX9BitmapTexture::~DX9BitmapTexture(void)
{
	UnloadResource();
	UnloadBitmap();
	pGraphicsService->RemoveResource(this);
}
void* mini3d::DX9BitmapTexture::GetBitmap(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;

	void* pBitmapCopy = malloc(sizeInBytes);
	memcpy(pBitmapCopy, pBitmap, sizeInBytes);
	
	return pBitmapCopy;
}

void mini3d::DX9BitmapTexture::SetBitmap(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth, const ITexture::WrapStyle wrapStyle)
{
	UnloadBitmap();
	
	sizeInBytes = width * height * 4;  // TODO: Depends on graphics settings
	this->pBitmap = malloc(sizeInBytes);
	memcpy(this->pBitmap, pBitmap, sizeInBytes);

	this->width = width;
	this->height = height;
	
	this->wrapStyle = wrapStyle;
	this->bitDepth = bitDepth;

	isDirty = true;
	LoadResource();
}

void mini3d::DX9BitmapTexture::UnloadBitmap(void)
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

void* mini3d::DX9BitmapTexture::Lock(unsigned int& sizeInBytes) const
{
	sizeInBytes = this->sizeInBytes;
	return pBitmap;
}
void mini3d::DX9BitmapTexture::Unlock(const bool& dataIsChanged)
{
	if (dataIsChanged)
	{
		isDirty = true;
		LoadResource();
	}
}

void mini3d::DX9BitmapTexture::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService->GetDevice();
	if (pDevice == 0)
		return;

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
		case IBitmapTexture::BIT_16:
			fmt = D3DFMT_X4R4G4B4;
			break;
		case IBitmapTexture::BIT_32:
			fmt = D3DFMT_X8R8G8B8;
			break;
		case IBitmapTexture::BIT_64:
			fmt = D3DFMT_A16B16G16R16;
			break;
		default:
			fmt = D3DFMT_X8R8G8B8;
		}

		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &pTexture, 0) ) ) 
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

	// Copy the data from pBitmap to d3dbuffer.
	// if the width (pitch) of the bitmap is small, it might be smaller than the d3dbuffer pitch so we have to take this into account when copying.
	unsigned int rowSizeInBytes = sizeInBytes / height;
	char* pD3DData;
	char* pBitmapData;
	
	for (unsigned int i = 0; i < height; i++)
	{
		pD3DData = (char*)textureDataRectangle.pBits + i * textureDataRectangle.Pitch;
		pBitmapData = (char*)pBitmap + width * i * 4; // TODO: depends on data type
		memcpy(pD3DData, pBitmapData, rowSizeInBytes);
	}

	pSurface->UnlockRect();
	pSurface->Release();

	bufferWidth = width;
	bufferHeight = height;
	isDirty = false;
}

void mini3d::DX9BitmapTexture::UnloadResource(void)
{
	if (pTexture != 0)
	{
		// if we are removing one of the current textures, clear that texture slot first
		for(int i = 0; i < pGraphicsService->GetMaxTextures(); i++)
			if (pGraphicsService->GetTexture(i) == this)
				pGraphicsService->SetTexture(0, i);

		pTexture->Release();
		pTexture = 0;
	}

	isDirty = true;
}