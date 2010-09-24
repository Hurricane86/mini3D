
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef AURORA_IVERTEXBUFFER_H
#define AURORA_IVERTEXBUFFER_H

namespace mini3d
{
/**
  IVertexBuffer is the interface for vertex buffers. Vertex buffers contains
  a description of the vertices that together with an index buffer defines
  3d geometry
  
  @see http://en.wikipedia.org/wiki/Polygon_mesh

  To create an instance of a IVertexBuffer, use the CreateVertexBuffer method in 
  IGraphicsService.
  
  To use the IVertexBuffer in rendering, bind it to the rendering pipeline using the 
  SetVertexBuffer method in IGraphicsService.
*/
class IVertexBuffer
{
public:

/**
	Destructor
*/
	virtual ~IVertexBuffer() {};

/**
	Returns a pointer to a copy of the vertex data in the buffer. 

	The data is a copy of the internal data in the buffer. The caller method is
	responsible for deleting the pointer once the data it is no longer needed.
	
	Brecause the internal data has to be copied before the new pointer is returned
	this method carries a performance hit.

	@see Use Lock method instead if you wish to manipulate the internal data directly.

	@param[out] sizeInBytes The size of the returned vertex data buffer.
	@return A void pointer to copy of vertex data.
 */
	virtual void* GetVertices(unsigned int& sizeInBytes) const = 0;

/**
	Sets the vertex data used to create the vertex buffer.
	
	@param pVertices Pointer to a buffer containing the vertex data for creating the buffer.
	@param count Number of vertices in the pVertices buffer.
	@param vertexSizeInBytes Size in bytes of a vertex in the pVertex buffre.
*/
	virtual void SetVertices(const void* pVertices, const unsigned int& count, const unsigned int& vertexSizeInBytes) = 0;

/**
	Lock the vertex data in the buffer to gain read and write access.

	The function will return a pointer to the actual data used to create the buffer 
	to allow read and write access. 

	@see Use GetVertexCount and GetVertexSizeInBytes methods to get information
	on how to address the vertex data in the buffer.

	@param[out] sizeInBytes Size of the vertex data buffer in bytes.
	@return Pointer to the internal vertex data buffer.
 */
	virtual void* Lock(unsigned int& sizeInBytes) const = 0;

/**
	Unlock the vertex data in the buffer to indicate that all reading or
	writing has been done.

	If the data in the buffer has been changed during the lock, this is indicated
	by setting the dataIsChanged parameter to true. If needed, this will trigger an
	update of the buffer to reflect changes in the data.

	Editing the data with the lock pointer after the buffer has been unlocked
	will lead to undefined behavior.

	Not setting the dataIsChanged parameter to true if the data has been modified will
	lead to undefined behavor.

	@param dataIsChanged indicates whether the caller has edited the contents of the
	buffer during the lock period.
 */	
	virtual void Unlock(const bool& dataIsChanged) = 0;

/**
	Gets the number of vertices in the buffer.
	@returns Number of vertices in the buffer.
 */
	virtual unsigned int GetVertexCount() const = 0;

/**
	Gets the size of one vertex in bytes.
	@returns Size of one vertex in bytes.
 */
	virtual unsigned int GetVertexSizeInBytes() const = 0;
};
}

#endif
