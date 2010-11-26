
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifndef MINI3D_ISHADERPROGRAM_H
#define MINI3D_ISHADERPROGRAM_H


namespace mini3d
{
class IPixelShader;
class IVertexShader;

/**
  IShaderProgram is the interface for a shader program. A shader program is a
  combination of a pixel shader and a vertex shader that together defines the
  programmable render stages in mini3d.
  
  @see IPixelShader
  @see IVertexShader
  @see http://en.wikipedia.org/wiki/Shader

  The shader language used for pixel and vertex shaders is implementation
  specific.

  To create an instance of a IShaderProgram, use the CreateShaderProgram method
  in IGraphicsService.
  
  To use the IShaderProgram in rendering bind it as the current shader program 
  using the SetShaderProgram method in IGraphicsService.
*/
class IShaderProgram
{
public:

/**
	Destructor
*/	
	virtual ~IShaderProgram(void) {};

/**
	Gets a pointer to the pixel shader used in this shader program

	There is no SetPixelShader, instead create a new shader program with the 
	desired pixel and vertex shader and delete the old one if it is no longer
	needed.
	
	@return Pointer to the pixel shader in this shader program.
 */
	virtual IPixelShader* GetPixelShader() const = 0;

/**
	Gets a pointer to the vertex shader used in this shader program

	There is no SetVertexlShader, instead create a new shader program with the 
	desired pixel and vertex shader and delete the old one if it is no longer
	needed.
	
	@return Pointer to the vertex shader in this shader program.
 */
	virtual IVertexShader* GetVertexShader() const = 0;
};
}

#endif
