
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_IPIXELSHADER_H
#define AURORA_IPIXELSHADER_H

namespace mini3d
{
/**
  IPixelShader is the interface for a pixel shader. A pixel shader is a program
  that runs on the GPU to determine the color output to bound render target when
  rendering geometry.

  @see http://en.wikipedia.org/wiki/Pixel_shader
   
  The shader language used for pixel shaders is implementation specific.

  To create an instance of a IPixelShader, use the CreatePixelShader method in 
  IGraphicsService.
  
  To use the IPixelShader in rendering, create an IShaderProgram combining the
  IPixelShader and an IVertexShader and bind that IShaderProgram as the current
  shader program using the SetShaderProgram method in IGraphicsService.
*/
class IPixelShader
{
	friend class IGraphicsDevice;

public:

/**
	Destructor
*/
	virtual ~IPixelShader() {};

/**
	Gets a pointer to a buffer containing the source for the pixel shader.

	There is no SetPixelShader function. Istead create a whole new new pixel
	shader if needed and delete the old one.

	@param[out] sizeInBytes Size of the pixel shader buffer in bytes.
	@return Pointer to a buffer containing the source for the pixel shader.
 */
	virtual void* GetPixelShader(unsigned int& sizeInBytes) = 0;
};
}

#endif
