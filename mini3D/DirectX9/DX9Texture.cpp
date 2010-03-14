#include "DX9Texture.h"
#include <d3d9.h>

DX9Texture::DX9Texture(DX9GraphicsService* pGraphicsService, void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, ITexture::BitDepth bitDepth) :
	pGraphicsService(pGraphicsService), bufferWidth(0), bufferHeight(0), pBitmap(0), pTexture(0)
{
	SetBitmap(pBitmap, width, height);
	LoadResource();
}

DX9Texture::~DX9Texture(void)
{
	UnloadResource();
	UnloadBitmap();
	pGraphicsService->RemoveResource(this);
}
IDirect3DTexture9* DX9Texture::GetTexture(void)
{
	return pTexture;
}
void* DX9Texture::GetBitmap(unsigned int& width, unsigned int& height, ITexture::WrapStyle& wrapStyle, ITexture::BitDepth& bitDepth)
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

	wrapStyle = ITexture::TILE;
	bitDepth = ITexture::BIT32;

	this->isDirty = true;

	return pReturnBitmap;
}
void DX9Texture::SetBitmap(void* pBitmap, unsigned int width, unsigned int height, ITexture::WrapStyle wrapStyle, ITexture::BitDepth bitDepth)
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
void DX9Texture::UnloadBitmap(void)
{
	if (pBitmap != 0)
		operator delete(pBitmap);

	pBitmap = 0;
	width = 0;
	height = 0;
	wrapStyle = ITexture::TILE;
	bitDepth = ITexture::BIT32;
}
unsigned int DX9Texture::GetWidth(void)
{
	return width;
}
unsigned int DX9Texture::GetHeight(void)
{
	return height;
}
ITexture::WrapStyle DX9Texture::GetWrapStyle(void)
{
	return wrapStyle;
}
ITexture::BitDepth DX9Texture::GetBitDepth(void)
{
	return bitDepth;
}
void DX9Texture::LoadResource(void)
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
		case ITexture::BIT16:
			fmt = D3DFMT_X4R4G4B4;
			break;
		case ITexture::BIT32:
			fmt = D3DFMT_X8R8G8B8;
			break;
		case ITexture::BIT64:
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

void DX9Texture::UnloadResource(void)
{
	if (pTexture != 0)
	{
		pTexture->Release();
		pTexture = 0;
	}

	isDirty = true;
}
bool DX9Texture::GetIsDirty(void)
{
	return isDirty;
}
