
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#include "OSWrapperLinux.h"
#include "math.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

// Init the OS
void Init() { }


// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

// Window Functions
void mini3d::OSWrapper::GetWindowContentSize(const MINI3D_WINDOW window, unsigned int &width, unsigned int &height) const
{
	Display* display = XOpenDisplay(0);

	XWindowAttributes attr;
	XGetWindowAttributes(display, window, &attr);
	
	width = attr.width;
	height = attr.height;
}

unsigned int mini3d::OSWrapper::GetMonitorBitDepth() const
{
	return 32;
}

void mini3d::OSWrapper::SetFullscreenWindow(MINI3D_WINDOW window, const unsigned int& width, const unsigned int& height)
{
	// Set the video resolution to the fullscreen resolution
	
	// Set window to correct size and style

}

void mini3d::OSWrapper::RestoreFullscreenWindow(MINI3D_WINDOW window)
{
	// Restore video resolution
	
	// restore window size and style
}

void mini3d::OSWrapper::Log(const char* message) const
{
	//printf();
};


// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

Display* mini3d::OSWrapper::GetMonitorNameFromWindow(const Window window)
{
	Display* display = XOpenDisplay(0);

	XWindowAttributes attr;
	XGetWindowAttributes(display, window, &attr);
	
	return attr.screen->display;
}

int mini3d::OSWrapper::GetClosestCompatibleGraphicsMode(const wchar_t monitorName[32], const unsigned int& width, const unsigned int& height)
{

}
	
unsigned int mini3d::OSWrapper::ScoreGraphicsModeMatch(const int dm1, const int dm2)
{
	// Score the similarity of the display modes by getting the difference between widths, heights and frequencies.
	// We get the total score when we add their absolute values together.
	//unsigned int score = (unsigned int)(abs((double)(dm1.dmPelsWidth - dm2.dmPelsWidth)) + 
//										abs((double)(dm1.dmPelsHeight - dm2.dmPelsHeight)) + 
//										abs((double)(dm1.dmDisplayFrequency - dm2.dmDisplayFrequency)));

	return 0;	
}

#endif