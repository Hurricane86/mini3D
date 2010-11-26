
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IVERTEXSHADER_H
#define MINI3D_IVERTEXSHADER_H

namespace mini3d
{
/**
  IvertexShader is the interface for a vertex shader. A vertex shader is a program
  that runs on the GPU and modifies geometry before it is used to draw to the
  render target. This way meshes can be dynamically altered before being rendered.

  @see http://en.wikipedia.org/wiki/Vertex_shader
   
  The shader language used for vertex shaders is implementation specific.

  To create an instance of a IVertexShader, use the CreateVertexShader method in 
  IGraphicsService.
  
  To use the IVertexShader in rendering, create an IShaderProgram combining the
  IVertexShader and a IPixelShader and bind that IShaderProgram as the current
  shader program using the SetShaderProgram method in IGraphicsService.
*/
class IVertexShader
{
public:

/**
	An enum describing the different possible vertex data types that makes up a
	vertex.

	A vertex consists of several data fields such as position, color, texture
	coordinates and so on... when creating a vertex shader it is required to specify
	the type of vertices that that shader operates on. This is done by creating a
	vertex declaration. A vertex declaration is an array of vertex data types
	describing the layout of data in a vertex.
*/
	enum VertexDataType { 
							POSITION_FLOAT3 = 0,
							POSITION_FLOAT4 = 1,
							COLOR_INT = 2, 
							COLOR_FLOAT4 = 3, 
							TEXTURECOORDINATE_FLOAT2 = 4, 
							NORMAL_FLOAT3 = 5, 
							NORMAL_FLOAT4 = 6, 
							BINORMAL_FLOAT3 = 7, 
							BINORMAL_FLOAT4 = 8, 
							TANGENT_FLOAT3 = 9, 
							TANGENT_FLOAT4 = 10, 
							BLENDWEIGHT_FLOAT = 11, 
							BLENDINDICES_INT = 12 
						};
	
public:
/**
	Destructor
*/
	virtual ~IVertexShader() {};

/**
	Gets a pointer to a buffer containing the source for the vertex shader.

	There is no SetVertexShader function. Istead create a whole new new vertex
	shader if needed and delete the old one.

	@param[out] sizeInBytes Size of the vertex shader buffer in bytes.
	@return Pointer to a buffer containing the source for the vertex shader.
 */
	virtual void* GetVertexShader(unsigned int& sizeInBytes) const = 0;

/**
	Gets the vertex declaration describing the vertex data layout expected by
	the vertex shader.

	@param[out] vertexDataCount Number of elements in the vertex declaration.
	@return Pointer to an array of type VertexDataType.
	@see VertexDataType
 */
	virtual VertexDataType* GetVertexDeclaration(unsigned int& vertexDatacount) const = 0;
};
}

#endif
