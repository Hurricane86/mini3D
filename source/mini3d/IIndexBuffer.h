
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifndef MINI3D_IINDEXBUFFER_H
#define MINI3D_IINDEXBUFFER_H

namespace mini3d
{
/**
  IIndexBuffer is the interface for an index buffer describing the order in which geometry
  in a vertex buffer should be drawn.
  
  @see http://en.wikipedia.org/wiki/Polygon_mesh

  To create an instance of a IIndexBuffer, use the CreateIndexBuffer method in 
  IGraphicsService.
  
  To use the IIndexBuffer in rendering, bind it to the rendering pipeline using the 
  SetIndexBuffer method in IGraphicsService.
*/
class IIndexBuffer
{
public:

/**
	A set backface culling orders for determining backface cull order.
	
	Backface culling is a technique where triangles facing away from the camera are
	not drawn. This is determined by the order of the triangle indices. If the vertex
	order of the triangle as seen from the viewport matches the cull order the triangle
	will not be drawn, saving GPU resources.

	@see http://en.wikipedia.org/wiki/Hidden_surface_determination
*/
	enum CullMode { CULL_CLOCKWIZE = 0, CULL_COUNTERCLOCKWIZE = 1, CULL_NONE = 2 };

/**
	The number of bits used for each index. The number of bits limits how many vertices
	can be used in the vertex buffer associated with this index buffer.
	For a 16 bit index buffer up to 65535 vertices can be addressed.
	For a 32 bit index buffer up to 4294967295 vertices can be addressed.
*/
	enum DataType { INT_16 = 0, INT_32 = 1 };

/**
	Destructor
*/
	virtual ~IIndexBuffer() {};

/**
	Returns a pointer to a copy of the index data in the buffer. 

	The data is a copy of the internal data in the buffer. The caller method is
	responsible for deleting the pointer once the data it is no longer needed.
	
	Brecause the internal data has to be copied before the new pointer is returned
	this method carries a performance hit.

	@see Use Lock method instead if you wish to manipulate the internal data directly.

	@param[out] sizeInBytes The size of the returned index data buffer.
	@return A void pointer to copy of index data.
 */
	virtual void* GetIndices(unsigned int& sizeInBytes) const = 0;

/**
	Sets the index data used to create the index buffer.
	
	@param pIndices Pointer to a buffer containing the index data for creating the buffer.
	@param count Number of indices in the pIndices buffer.
	@param dataType Data type for the indices (how many bits per index), default is 32 bits per index.
	@param cullMode Cull mode to be used when drawing geometry using this IIndexBuffer, default is counter clockwize
	@see IIndexBuffer::DataType
	@see IIndexBuffer::CullMode
*/
	virtual void SetIndices(const void* pIndices, const unsigned int& count, const DataType& dataType = INT_32, const CullMode& cullMode = CULL_COUNTERCLOCKWIZE) = 0;

/**
	Lock the index data in the buffer to gain read and write access.

	The function will return a pointer to the actual data used to create the buffer 
	to allow read and write access. 

	@see Use GetDataType method to get the data type of the indices.
	@see Use GetIndices method if you want a copy of the internal bitmap data
	instead.

	@param[out] sizeInBytes Size of the index data buffer in bytes.
	@return Pointer to the internal index data buffer.
 */
	virtual void* Lock(unsigned int& sizeInBytes) const = 0;

/**
	Unlock the index data in the buffer to indicate that all reading or
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
	Gets the number of indices in the index buffer.
	@returns Number of indices in the index buffer.
 */
	virtual unsigned int GetIndexCount() const = 0;

/**
	Gets the data type of the indices in the index buffer.
	@returns Data type of indices in the index buffer.
	@see DataType
 */
	virtual DataType GetDataType() const = 0;

/**
	Gets the current cull mode for the index buffer.
	@returns Cullmode of the index buffer.
	@see DataType
 */
	virtual CullMode GetCullMode() const = 0;

	/**
	Sets the current cull mode for the index buffer.
	@param cullmode Cullmode of the index buffer.
	@see CullMode
 */
	virtual void SetCullMode(const CullMode& cullMode) = 0;
};
}

#endif
