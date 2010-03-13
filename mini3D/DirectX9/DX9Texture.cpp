#include "DX9Texture.h"
#include <d3d9.h>

DX9Texture::DX9Texture(DX9GraphicsService* pGraphicsService, void* pBitmap, unsigned int width, unsigned int height) :
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
void* DX9Texture::GetBitmap(unsigned int& width, unsigned int& height)
{
	void* pReturnBitmap = pBitmap;
	width = this->width;
	height = this->height;

	// reset the bitmap information because we are "removing" the bitmap from the texture when we get it!
	this->width = 0;
	this->height = 0;
	this->pBitmap = 0;

	this->isDirty = true;

	return pReturnBitmap;
}
void DX9Texture::SetBitmap(void* pBitmap, unsigned int width, unsigned int height)
{
	UnloadBitmap();
	
	int sizeInBytes = width * height * 4;  // TODO: Depends on graphics settings
	this->pBitmap = pBitmap;

	this->width = width;
	this->height = height;

	isDirty = true;
}
void DX9Texture::UnloadBitmap(void)
{
	if (pBitmap != 0)
		free(pBitmap);

	pBitmap = 0;
	width = 0;
	height = 0;
}
unsigned int DX9Texture::GetWidth()
{
	return width;
}
unsigned int DX9Texture::GetHeight()
{
	return height;
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
		if( FAILED( pDevice->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pTexture, 0 ) ) ) 
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