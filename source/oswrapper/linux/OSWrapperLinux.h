
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifdef __linux

#ifndef MINI3D_OSWRAPPERLINUX_H
#define MINI3D_OSWRAPPERLINUX_H

// Set the data type for the MINI3D_WINDOW macro
#include "../IOSWrapper.h"

// Set platform dependent types
namespace mini3d
{
class OSWrapper : IOSWrapper
{
public:

	// ::::: Constructor and Destructor :::::::::::::::::::::::::::::::::::::::

	OSWrapper() {};
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
	
	Display* GetMonitorNameFromWindow(Window window);
	unsigned int ScoreGraphicsModeMatch(const int dm1, const int dm2);
	int GetClosestCompatibleGraphicsMode(const wchar_t monitorName[32], const unsigned int& width, const unsigned int& height);

	
	// ::::: Member Variables :::::::::::::::::::::::::::::::::::::::::::::::::

};
}

#endif
#endif
