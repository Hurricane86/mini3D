
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IBITMAPTEXTURE_H
#define MINI3D_IBITMAPTEXTURE_H

#include "internal/ITexture.h"

namespace mini3d
{

/**
  IBitmapTexture is the interface for a texture created from a bitmap data source.
   
  To create an instance of a IBitmapTexture, use the CreateBitmapTexture method in 
  IGraphicsService.
  
  To use the IBitmapTexture bind it to a textur slot in using the SetTexture function
  in IGraphicsService.
 
  For the texture to be used in rendered objects a texture lookup must be done in the
  pixel shader and the resulting color must be used as part of the color output for
  the shader.
*/
class IBitmapTexture : public virtual ITexture
{
public:

/**
	The bit depth of the bitmap texture. 
	The function arguments listed with "param" will be compared 
	to the declaration and verified.
 */    
	enum BitDepth {	BIT_16, BIT_32, BIT_64 };

/**
	Destructor. 
 */    
	virtual ~IBitmapTexture() {};

/**
	Returns a pointer to a copy of the bitmap data in the bitmap texture. 

	The data is a copy of the internal data in the bitmap texture. The caller 
	method is responsible for deleting the pointer once it is no longer needed.  
	
	Brecause the internal data has to be copied before the new pointer is returned
	this method carries a performance hit.

	@see Use Lock method instead if you wish to manipulate the internal data directly.

	@param[out] sizeInBytes The size of the returned bitmap data buffer.
	@return A void pointer to copy of bitmap data.
 */
	virtual void* GetBitmap(unsigned int& sizeInBytes) const = 0;

/**
	Sets the bitmap data used to create the bitmap texture.

	The data provided will be copied to an internal buffer.   

	@param pBitmap Pointer to a buffer containing the bitmap data to be used for creating the texture.
	@param width Width of the bitmap described by the bitmap data.
	@param height Height of the bitmap described by the bimap data.
	@param bitDepth Number of bits per pixel for the bitmap data.
	@param wrapStyle How the texture will be repeated if it is tiled.
 */
	virtual void SetBitmap(const void* pBitmap, const unsigned int& width, const unsigned int& height, const IBitmapTexture::BitDepth bitDepth = IBitmapTexture::BIT_32, const ITexture::WrapStyle wrapStyle = ITexture::WRAP_TILE) = 0;

/**
	Lock the bitmap data in the texture to gain read and write access.

	The function will return a pointer to the actual bitmap data used to create
	the texture to allow read and write access. 

	@see Use GetBitDepth method to get the bits per pixel of the bitmap data.
	@see Use GetBitmap method if you want a copy of the internal bitmap data
	instead.

	@param[out] sizeInBytes Size of the bitmap data buffer in bytes.
	@return Pointer to the internal bitmap data buffer.
 */
	virtual void* Lock(unsigned int& sizeInBytes) const = 0;

/**
	Unlock the bitmap data in the bitmap texture to indicate that all reading or
	writing has been done.

	If the data in the buffer has been changed during the lock, this is indicated
	by setting the dataIsChanged parameter to true. If needed, this will trigger an
	update of the bitmap texture to reflect changes in the bitmap data.

	Editing the data with the lock pointer after the bitmap texture has been unlocked
	will lead to undefined behavior.

	Not setting the dataIsChanged parameter to true if the data has been modified will
	lead to undefined behavor.

	@param dataIsChanged indicates whether the caller has edited the contents of the
	buffer during the lock period.
 */
	virtual void Unlock(const bool& dataIsChanged = false) = 0;

/**
	Get the width of the bitmap texture.

	@return Width of the bitmap texture
 */
	virtual unsigned int GetWidth() const = 0;

/**
	Get the height of the bitmap texture.

	@return height of the bitmap texture
 */
	virtual unsigned int GetHeight() const = 0;

/**
	Get the strategy for repeating the texture when it is tiled.

	@return Wrap style for the texture
 */
	virtual WrapStyle GetWrapStyle() const = 0;

/**
	Set the strategy for repeating the texture when it is tiled.

	@return wrapStyle the wrap style to use for the texture
 */
	virtual void SetWrapStyle(const WrapStyle& wrapStyle) = 0;

/**
	Get the number of bits per pixel for the bitmap texture

	@return Number of bits per pixel for the bitmap texture
 */
	virtual BitDepth GetBitDepth() const = 0;
};
}

#endif
