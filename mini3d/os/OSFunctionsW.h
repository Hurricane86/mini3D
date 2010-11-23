
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifndef MINI3D_OSFUNCTIONW_H
#define MINI3D_OSFUNCTIONW_H

// Set the data type for the MINI3D_WINDOW macro
#include "../datatypes/DataTypes.h"

// Set platform dependent types
#ifdef _WIN32
typedef HMONITOR MINI3D_SCREEN;
typedef int MINI3D_GRAPHICS_CARD;
typedef DEVMODE GRAPHICS_MODE;
#endif

namespace mini3d
{
class OSFunctionW
{
public:

	// ::::: Constructor and Destructor :::::::::::::::::::::::::::::::::::::::

	OSFunctionW();
	virtual ~OSFunctionW() {};

	// Init the OS
	void Init();


	// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

	// Window Functions
	void GetWindowSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const;
	void GetWindowClientSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const;
	MINI3D_SCREEN GetScreenFromWindow(const MINI3D_WINDOW window) const;

	// Get screen information
	void GetScreenPosition(const MINI3D_SCREEN screen, unsigned int& x, unsigned int& y) const;
	void GetScreenResolution(const MINI3D_SCREEN screen, unsigned int& width, unsigned int& height ) const;
	unsigned int GetScreenBitDepth(const MINI3D_SCREEN screen) const;
	unsigned int GetScreenFrequency(const MINI3D_SCREEN screen) const;
	unsigned int GetScreenOrientation(const MINI3D_SCREEN screen) const;
	MINI3D_GRAPHICS_CARD GetGraphicsCardFromScreen(const MINI3D_SCREEN screen) const;

	// Get Graphics Card Information
	int GetGraphicsCardCount() const;
	
	// Debug function
	void PrintAllGraphicsCardInformation() const;
	
	// Window States
	virtual void SetFullscreenWindow(MINI3D_WINDOW hWindow, const unsigned int& width, const unsigned int& height);
	virtual void RestoreFullscreenWindow(MINI3D_WINDOW hWindow);

	// Debug log output
	void Log(char* message) const;


private:
	
	// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

	GRAPHICS_MODE GetClosestCompatibleGraphicsMode(MINI3D_WINDOW hWindow, const unsigned int& width, const unsigned int& height);
	unsigned int ScoreGraphicsModeMatch(const GRAPHICS_MODE dm1, const GRAPHICS_MODE dm2);

	
	// ::::: Member Variables :::::::::::::::::::::::::::::::::::::::::::::::::

	// Current fullscreen window
	HWND fullscreenWindow;

	// Stores the original window position and size before entering fullscreen
	RECT winRect;

	// Stores the windowstyle for the window before changing it when entering fullscreen
	long windowStyle;
};
}

#endif