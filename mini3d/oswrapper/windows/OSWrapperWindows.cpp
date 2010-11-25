
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "OSWrapperWindows.h"
#include "math.h"

// Init the OS
void Init() { }


// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

//void mini3d::OSFunctionW::GetWindowClientSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const
//{
//	RECT rect;
//	GetClientRect(windowHandle, &rect);
//
//	// get the width and height (must be bigger than 0)
//	width = (rect.right - rect.left) | 1;
//	height = (rect.bottom - rect.top) | 1;
//}
//
//MINI3D_SCREEN mini3d::OSFunctionW::GetScreenFromWindow(const MINI3D_WINDOW window) const
//{
//	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
//	return monitor;
//}
//
//// Get screen information
//void mini3d::OSFunctionW::GetScreenPosition(const MINI3D_SCREEN screen, unsigned int& x, unsigned int& y) const
//{
//	MONITORINFO info;
//	info.cbSize = sizeof(MONITORINFO);
//	GetMonitorInfo(screen, &info);
//
//	x = info.rcMonitor.left;
//	y = info.rcMonitor.top;
//}
//void mini3d::OSFunctionW::GetScreenResolution(const MINI3D_SCREEN screen, unsigned int& width, unsigned int& height ) const
//{
//	MONITORINFOEX info;
//	info.cbSize = sizeof(MONITORINFOEX);
//	GetMonitorInfo(screen, &info);
//		
//	DEVMODE dm;
//	dm.dmSize = sizeof(DEVMODE);
//	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);
//
//	width = dm.dmPelsWidth;
//	height = dm.dmPelsHeight;
//}
//unsigned int mini3d::OSFunctionW::GetScreenBitDepth(const MINI3D_SCREEN screen) const
//{
//	MONITORINFOEX info;
//	info.cbSize = sizeof(MONITORINFOEX);
//	GetMonitorInfo(screen, &info);
//		
//	DEVMODE dm;
//	dm.dmSize = sizeof(DEVMODE);
//	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);
//
//	return dm.dmBitsPerPel;
//}
//unsigned int mini3d::OSFunctionW::GetScreenFrequency(const MINI3D_SCREEN screen) const
//{
//	MONITORINFOEX info;
//	info.cbSize = sizeof(MONITORINFOEX);
//	GetMonitorInfo(screen, &info);
//		
//	DEVMODE dm;
//	dm.dmSize = sizeof(DEVMODE);
//	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);
//
//	return dm.dmDisplayFrequency;		
//}
//unsigned int mini3d::OSFunctionW::GetScreenOrientation(const MINI3D_SCREEN screen) const
//{
//	MONITORINFOEX info;
//	info.cbSize = sizeof(MONITORINFOEX);
//	GetMonitorInfo(screen, &info);
//		
//	DEVMODE dm;
//	dm.dmSize = sizeof(DEVMODE);
//	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);
//
//	return dm.dmDisplayOrientation;
//}
//MINI3D_GRAPHICS_CARD mini3d::OSFunctionW::GetGraphicsCardFromScreen(const MINI3D_SCREEN screen) const
//{
//	MONITORINFOEX monitorInfo;
//	monitorInfo.cbSize = sizeof(MONITORINFOEX);
//	GetMonitorInfo(screen, &monitorInfo);
//		
//	// Enumerate all display adapters.
//	DISPLAY_DEVICE device, monitor;
//	device.cb = monitor.cb = sizeof(DISPLAY_DEVICE);
//		
//	DWORD devNum = 0;
//	while(EnumDisplayDevices(0, devNum, &device, 0)) 
//	{
//		// Loop through all monitors this display adapter is attached to (in case it is multi headed).
//		DWORD monitorNum = 0;
//		while(EnumDisplayDevices(device.DeviceName, monitorNum, &monitor, 0))
//		{
//			if (monitor.DeviceName == monitorInfo.szDevice)
//				return devNum;
//
//			monitorNum++;
//		}
//		devNum++;
//	}
//	return 0;
//}
//
//// Get Graphics Card Information
//int mini3d::OSFunctionW::GetGraphicsCardCount() const
//{
//	// TODO: This stuff we should really cache and update when there is a screen change!
//
//	// Enumerate all display adapters.
//	DISPLAY_DEVICE device;
//	device.cb = sizeof(DISPLAY_DEVICE);
//		
//	int numDevices = 0;
//
//	// Loop through all monitors this display adapter is attached to (in case it is multi headed).
//	DWORD deviceNum = 0;
//	while(EnumDisplayDevices(0, deviceNum, &device, 0))
//	{
//		deviceNum++;
//
//		// Skip this device if it is a mirroring device (such as the NetMeetingvdevice) or if it is not attached to the desktop.
//		if((device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) || !(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
//			continue;
//
//		// count this monitor
//		numDevices++;
//	}
//
//	MINI3D_GRAPHICS_CARD* graphicsCards = new MINI3D_GRAPHICS_CARD[numDevices];
//
//	numDevices = deviceNum = 0;
//	while(EnumDisplayDevices(0, deviceNum, &device, 0))
//	{
//		deviceNum++;
//
//		// Skip this device if it is a mirroring device (such as the NetMeetingvdevice) or if it is not attached to the desktop.
//		if((device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) || !(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
//			continue;
//
//		// count this monitor
//		numDevices++;
//	}
//		
//	return numDevices;
//}		
//	
//// Debug function
//void mini3d::OSFunctionW::PrintAllGraphicsCardInformation() const
//{
//	// Enumerate all display adapters.
//	DISPLAY_DEVICE device;
//	device.cb = sizeof(DISPLAY_DEVICE);
//	DWORD devNum = 0;
//	while(EnumDisplayDevices(0, devNum, &device, 0)) 
//	{
//		// Make sure this isn't a mirroring device (such as the NetMeetingvdevice) and is attached to the desktop.
//		if(!(device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) && (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) 
//		{
//			Log("Device Information -----------");
//			Log((char*)device.DeviceID);
//			Log((char*)"\n");
//			Log((char*)device.DeviceKey);
//			Log((char*)"\n");
//			Log((char*)device.DeviceName);
//			Log((char*)"\n");
//			Log((char*)device.DeviceString);
//			Log((char*)"\n");
//
//			// Loop through all monitors this display adapter is attached to (in case it is multi headed).
//			DISPLAY_DEVICE monitor;
//			monitor.cb = sizeof(DISPLAY_DEVICE);
//			   
//			DWORD monitorNum = 0;
//			while(EnumDisplayDevices(device.DeviceName, monitorNum, &monitor, 0)) 
//			{
//				// Make sure this isn't a mirroring device (such as the NetMeeting device) and is attached to the desktop.
//				if(!(monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) && (monitor.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) 
//				{
//				Log("Screen Information -----------");
//				Log((char*)monitor.DeviceID);
//				Log((char*)"\n");
//				Log((char*)monitor.DeviceKey);
//				Log((char*)"\n");
//				Log((char*)monitor.DeviceName);
//				Log((char*)"\n");
//				Log((char*)monitor.DeviceString);
//				Log((char*)"\n");
//				}
//
//				monitorNum += 1;
//			}
//		}
//		devNum += 1;
//	}
//}


// Window Functions
void mini3d::OSWrapper::GetWindowSize(const MINI3D_WINDOW window, unsigned int &width, unsigned int &height) const
{
	RECT rect;
	GetClientRect(window, &rect);

	// get the width and height (must be bigger than 0)
	width = (rect.right - rect.left) | 1;
	height = (rect.bottom - rect.top) | 1;
}

void mini3d::OSWrapper::SetFullscreenWindow(MINI3D_WINDOW window, const unsigned int& width, const unsigned int& height)
{
	// Find and store the fullscreen monitor name
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST), &monitorInfo);

	memcpy(fullscreenMonitorName, &monitorInfo.szDevice, sizeof(monitorInfo.szDevice));

	// Get the Device Mode that is closest to the requested resolution
	DEVMODE dm = GetClosestCompatibleGraphicsMode(fullscreenMonitorName, width, height);

	// Set the device mode position based on the current monitor coordinates in the virtual desktop
	dm.dmPosition.x = monitorInfo.rcMonitor.left;
	dm.dmPosition.y = monitorInfo.rcMonitor.top;

	// If we are already in fullscreen mode and the requested resolution is the same as the current one, dont set it again.
	if ((fullscreenWindow != 0) && (fullscreenDeviceMode.dmPelsWidth == dm.dmPelsWidth) && (fullscreenDeviceMode.dmPelsHeight == dm.dmPelsHeight))
		return;

	// Capture the device mode
	fullscreenDeviceMode = dm;

	// if this is the first time we enter fullscreen state, capture the settings
	if (fullscreenWindow == 0)
	{
		// Capture window style
		windowStyle = GetWindowLongPtr(window, GWL_STYLE);

		// Capture window position
		GetWindowRect(window, &winRect);

		// Keep track of new fullscreen window
		fullscreenWindow = window;
	}

	// Change Screen Resolution
	if (ChangeDisplaySettingsEx(fullscreenMonitorName, &dm, 0, CDS_FULLSCREEN, 0) != DISP_CHANGE_SUCCESSFUL)
	{      
		// TODO: Handle Error?
		return;
	}

	// TODO: Fix mouse cursor position (SetCursorPos, GetCursorPos)

	// Make the window fullscreen and the same size as the fullscreen desktop
	SetWindowLongPtr(window, GWL_STYLE, 0);
	SetWindowPos(window, HWND_TOPMOST, dm.dmPosition.x, dm.dmPosition.y, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

}
void mini3d::OSWrapper::RestoreFullscreenWindow(MINI3D_WINDOW window)
{
	// Only set if we are not already in windowed mode
	if (fullscreenWindow == 0)
		return;

	// Restore desktop resolution
	ChangeDisplaySettingsEx(fullscreenMonitorName, 0, 0, 0, 0);

	// Restore window style, size and position
	SetWindowLongPtr(window, GWL_STYLE, windowStyle);
	SetWindowPos(window, HWND_TOP, winRect.left, winRect.top, winRect.right - winRect.left, winRect.bottom - winRect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Set the fullscreenWindow to 0
	fullscreenWindow = 0;
}

void mini3d::OSWrapper::Log(const char* message) const
{
	OutputDebugStringA(message);
};


// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

void mini3d::OSWrapper::GetMonitorNameFromWindow(const HWND window, WCHAR* monitorName)
{
	// Get the monitor from the window
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);

	// Get the monitor name
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(monitor, &info);
	
	// Copy the result to the return string
	memcpy(monitorName, &info.szDevice, sizeof(info.szDevice));
}

DEVMODE mini3d::OSWrapper::GetClosestCompatibleGraphicsMode(const WCHAR monitorName[32], const unsigned int& width, const unsigned int& height)
{

	// Get the current device mode
	DEVMODE currentDM;
	currentDM.dmSize = sizeof(currentDM);
	EnumDisplaySettings(monitorName, ENUM_CURRENT_SETTINGS, &currentDM);

	// inel

	// If width or height = 0 return the current device mode
	if (width == 0 || height == 0)
		return currentDM;

	// initialize the device mode structure for requested device mode
	DEVMODE requestedDM = currentDM;
	requestedDM.dmPelsWidth = width;
	requestedDM.dmPelsHeight = height;

	// initialize the device mode structure for best match
	DEVMODE bestDM = currentDM;

	// Score of the currentDM vs the requestedDM
	unsigned int bestMatch = ScoreGraphicsModeMatch(requestedDM, currentDM); 

	// initialize the device mode structure for looping over all device modes
	DEVMODE dm;
	dm.dmSize = sizeof(dm);

	// loop variable
	unsigned int i = 0;

	// Loop over all display settings and find the best match
	// EnumDisplaySettings returns 0 when we request a displaymode id that is out of range
	while (EnumDisplaySettings(monitorName, i++, &dm) != 0)
	{
		// skip modes with wrong color bit depth
		if (dm.dmBitsPerPel != currentDM.dmBitsPerPel)
			continue;

		// skip modes with wrong display orientation
		if (dm.dmDisplayOrientation != currentDM.dmDisplayOrientation)
			continue;

		// Score this match
		unsigned int match = ScoreGraphicsModeMatch(requestedDM, dm);

		// If this is the best match so far, update the match parameters
		if (match < bestMatch)
		{
			bestDM = dm;
			bestMatch = match;
		}
	}

	// Return the best match found
	return bestDM;
}
	
unsigned int mini3d::OSWrapper::ScoreGraphicsModeMatch(const DEVMODE dm1, const DEVMODE dm2)
{
	// Score the similarity of the display modes by getting the difference between widths, heights and frequencies.
	// We get the total score when we add their absolute values together.
	unsigned int score = (unsigned int)(abs((double)(dm1.dmPelsWidth - dm2.dmPelsWidth)) + 
										abs((double)(dm1.dmPelsHeight - dm2.dmPelsHeight)) + 
										abs((double)(dm1.dmDisplayFrequency - dm2.dmDisplayFrequency)));

	return score;	
}

#endif