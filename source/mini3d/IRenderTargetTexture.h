
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IRENDERTARGETTEXTURE_H
#define MINI3D_IRENDERTARGETTEXTURE_H

#include "internal/ITexture.h"
#include "internal/IRenderTarget.h"

namespace mini3d
{
	
/**
  IRenderTargetTexture is the interface for a render target texture.

  A render target texture is a texture that can also be used as a render target.

  A render target texture can not be bound as both a render target and a texture at
  the same time. If a render target texture is currently bound as a render target
  and then bound as a texture it will be unbound as a render target and vice versa.
   
  To create an instance of a IRenderTargetTexture, use the CreateRenderTargetTexture
  method in IGraphicsService.
  
  To use the IRenderTargetTexture as a texture, bind it to a textur slot in using the
  SetTexture function in IGraphicsService.

  To use the IRenderTargetTexture as a render target, bind it using the SetRenderTarget
  function in IGraphicsService.

  For the texture to be used in rendered objects a texture lookup must be done in the
  pixel shader and the resulting color must be used as part of the color output for
  the shader.
*/

class IRenderTargetTexture : public virtual IRenderTarget, public virtual ITexture
{
public:
/**
	Destructor
*/
	virtual ~IRenderTargetTexture() {};

/**
	Configures the render target texture.
	
	@param width Width of the render target texture.
	@param height Height of the render target texture.
	@param depthTestEnabled Determine if the graphics service should use a depth buffer when rendering
	to the render target. Enabeling depth testing hides geometry that is hidden behind other geometry.
	If depth testing is disabled triangles will draw on top of each other depending on the order in which
	they are drawn and not depending on their depth.

	@see http://en.wikipedia.org/wiki/Z-buffering
*/
	virtual void SetRenderTargetTexture(const unsigned int& width, const unsigned int& height, const bool& depthTestEnabled) = 0;

/**
	Get the width of the render target texture.

	@return Width of the render target texture
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
	Gets a boolean determining if depth test is enabled.

	@return A boolean determining if depth test is enabled
	@see SetRenderTargetTexture
 */
	virtual bool GetDepthTestEnabled() const = 0;

};
}

#endif
