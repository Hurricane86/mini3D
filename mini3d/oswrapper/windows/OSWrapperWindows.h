
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
	void GetWindowSize(const MINI3D_WINDOW window, unsigned int &width, unsigned int &height) const;

	// Window States
	void SetFullscreenWindow(MINI3D_WINDOW window, const unsigned int& width, const unsigned int& height);
	void RestoreFullscreenWindow(MINI3D_WINDOW window);
	void Log(const char* message) const;

	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	// Window Functions
	
	//void GetWindowClientSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const;
	//MINI3D_SCREEN GetScreenFromWindow(const MINI3D_WINDOW window) const;

	//// Get screen information
	//void GetScreenPosition(const MINI3D_SCREEN screen, unsigned int& x, unsigned int& y) const;
	//void GetScreenResolution(const MINI3D_SCREEN screen, unsigned int& width, unsigned int& height ) const;
	//unsigned int GetScreenBitDepth(const MINI3D_SCREEN screen) const;
	//unsigned int GetScreenFrequency(const MINI3D_SCREEN screen) const;
	//unsigned int GetScreenOrientation(const MINI3D_SCREEN screen) const;
	//MINI3D_GRAPHICS_CARD GetGraphicsCardFromScreen(const MINI3D_SCREEN screen) const;

	// Get Graphics Card Information
	//int GetGraphicsCardCount() const;
	
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
