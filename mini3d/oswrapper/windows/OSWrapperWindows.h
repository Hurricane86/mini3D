
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifdef _WIN32

#ifndef MINI3D_OSWRAPPERWINDOWS_H
#define MINI3D_OSWRAPPERWINDOWS_H

// Set the data type for the MINI3D_WINDOW macro
#include "../IOSWrapper.h"

// Set platform dependent types

namespace mini3d
{
class OSWrapper : IOSWrapper
{
public:

	// ::::: Constructor and Destructor :::::::::::::::::::::::::::::::::::::::

	OSWrapper() : fullscreenWindow(0) {};
	virtual ~OSWrapper() {};


	// ::::: IOSWrapper Methods :::::::::::::::::::::::::::::::::::::::::::::::

	// Window Dimensions
	void GetWindowContentSize(const MINI3D_WINDOW window, unsigned int &width, unsigned int &height) const;
	unsigned int GetMonitorBitDepth() const;

	// Window States
	void SetFullscreenWindow(MINI3D_WINDOW window, const unsigned int& width, const unsigned int& height);
	void RestoreFullscreenWindow(MINI3D_WINDOW window);

	// Logging
	void Log(const char* message) const;


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	// Debug function
	//void PrintAllGraphicsCardInformation() const;


private:
	
	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::
	
	void GetMonitorNameFromWindow(HWND window, WCHAR* monitorName);
	unsigned int ScoreGraphicsModeMatch(const DEVMODE dm1, const DEVMODE dm2);
	DEVMODE GetClosestCompatibleGraphicsMode(const WCHAR monitorName[32], const unsigned int& width, const unsigned int& height);

	
	// ::::: Member Variables :::::::::::::::::::::::::::::::::::::::::::::::::

	// Current fullscreen window
	HWND fullscreenWindow;

	// The device mode that the fullscreen was created with
	DEVMODE fullscreenDeviceMode;

	// Stores the original window position and size before entering fullscreen
	RECT winRect;

	// Stores the windowstyle for the window before changing it when entering fullscreen
	long windowStyle;

	// Stores the monitor handle for which we created the fullscreen window
	WCHAR fullscreenMonitorName[32];
};
}

#endif
#endif
