
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IWINDOWRENDERTARGET_H
#define MINI3D_IWINDOWRENDERTARGET_H

#include "internal\IRenderTarget.h"

namespace mini3d
{
		
/**
  IScreenRenderTarget is the interface for a screen render target texture.

  A screen render target is a render target that can be output to the monitor
  or screen (depending on the type of device used).

  The reference of an os window (such as a window handle) is used to identify the
  window that the IWindowRenderTarget should use to create itself. The
  IWindowRenderTarget is the bridge that directs whatever is output by graphics
  hardware to the window the result should be displayed in.

  To create an instance of a IScreenRenderTarget, use the CreateScreenRenderTarget
  method in IGraphicsService.
  
  To use the IScreenRenderTarget as a texture, bind it to a textur slot in using the
  SetRenderTarget function in IGraphicsService.
  @see CreateScreenRenderTarget
*/
class IWindowRenderTarget : public virtual IRenderTarget
{
public:

/**
	Enum describing if the screen render target is in fullscreen state or windowed state
*/
	enum ScreenState { SCREEN_STATE_WINDOWED = 0, SCREEN_STATE_FULLSCREEN = 1 };

/**
	Enum describing the rendering quality used when rendering to the render target.
	For most implemenations this will correspond to antialiasing and related setings.

	QUALITY_MINIMUM setting corresponds to no multisampling and no anisotropic filtering.
	QUALITY_LOW setting corresponds to the lowest multisampling available and no
	Anisotropic filtering.
	QUALITY_MEDIUM corresponds to a well balanced multisample and anisotropic filtering settings.
	QUALITY_HIGH corresponds to a high multisaple and anisotropic filtering setting.
	QUALITY_MAXIMUM corresponds to both multisampe and anisotropic filtering settings being maxed out.

	For best performance use QUALITY_MINIMUM.
	For best performance with some multisample and anisotropic filtering, use QUALITY_LOW;
	For well balanced high quality graphics, use the QUALITY_MEDIUM setting.
	For applications where quality is of higher importance than performance, use QUALITY_HIGH or QUALITY_MAXIMUM.

	This is a highly implementation and hardware dependent setting. If the graphics hardware offers free
	antialiasing even the minimum quality setting may use it.

*/
	enum Quality{QUALITY_MINIMUM, QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH, QUALITY_MAXIMUM};

/**
	Destructor
*/
	virtual ~IWindowRenderTarget() {};

/**
	Sets the parameters for creating the screen render target
	 
	@param windowHandle Operating system specific handle to the widget that shall define the region
	of the render target. This could be a window or a graphical component depending on the implementaion
	and the operating system used.
	@param depthTestEnabled Determine if the graphics service should use a depth buffer when rendering
	to the render target. Enabeling depth testing hides geometry that is hidden behind other geometry.
	If depth testing is disabled triangles will draw on top of each other depending on the order in which
	they are drawn and not depending on their depth.
	@param quality The quality level that should be used for drawing.
	@see Quality
*/
	virtual void SetWindowRenderTarget(const int& windowHandle, const bool& depthTestEnabled, const Quality& quality) = 0;

/**
	Get the width of the render target in windowed mode.

	@return Width of the render target in windowed mode.
 */
	virtual unsigned int GetWidth() const = 0;

/**
	Get the height of the render target in windowed mode.

	@return Height of the render target in windowed mode.
 */
	virtual unsigned int GetHeight() const = 0;

/**
	Gets a boolean determining if depth test is enabled.

	@return A boolean determining if depth test is enabled
	@see SetRenderTargetTexture
 */
	virtual bool GetDepthTestEnabled() const = 0;

/**
	Get the quality level used for rendering to the screen render target

	@return Quality level used for rendering to the screen render target
	@see Quality
 */
	virtual Quality GetQuality() const = 0;

/**
	The handle to the widget that contains the window render target.

	@return Handle to the widget that contains the window render target.
	@see SetWindowRenderTarget
 */
	virtual int GetWindowHandle() const = 0;

/**
	Gets the current screen state.

	@return Current screen state.
	@see ScreenState
 */
	virtual ScreenState GetScreenState() const = 0;

/**
	Sets the current screen state to windowed screen state mode.
	The render target window will assume the same size it had before going into fullscreen mode.
	It is not possible to resize the render target window while in fullscreen mode. It is only
	possible to resize the window while in windowed mode
	
	@see ScreenState
 */
	virtual void SetScreenStateWindowed() = 0;

/**
	Sets the current screen state to fullscreen. This will make the render target window cover the
	whole screen and possibly get exclusive access to the rendering hardware depending on
	implementation.
	
	When going into fullscreen mode it is possible to set the horizontal and vertical resolution
	for the fullscreen mode.

	If either fullscreenWidth or fullscreenHeight is set 0 or both fullscreenWidth and fullscreenHeight
	are set to 0 the current screen resolution will be used as resolution in fullscreen mode.

	If the given width and height does not make up a valid fullscreen mode for the graphics device
	the closest valid lower resolution will be used. If no valid lower resolution can be find the
	closest valid higher resolution will be used.

	If that resolution is not valid, the next valid lower resolution will be used, and so on...

	@param fullscreenWidth Width of the render target in fullscreen mode.
	@param fullscreenHeight Height of the render target in fullscreen mode.
	@see ScreenState
*/
	virtual void SetScreenStateFullscreen(const unsigned int& fullscreenWidth, const unsigned int& fullscreenHeight) = 0;

/**
	This function shall be called when all rendering to the screen render target
	is done and the render target shall be shown on the screen.

	Rendering to render targets is buffered, this means that the rendering is done
	off screen and the result is not shown until the method Display is called.
*/
	virtual void Display() = 0;
};
}

#endif
