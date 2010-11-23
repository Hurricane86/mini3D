	
#include "OSFunctionsW.h"
#include "math.h"

// Init the OS
void Init() { }


// ::::: Public Methods :::::::::::::::::::::::::::::::::::::::::::::::::::

// Window Functions
void mini3d::OSFunctionW::GetWindowSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const
{
	RECT rect;
	GetClientRect(windowHandle, &rect);

	// get the width and height (must be bigger than 0)
	width = (rect.right - rect.left) | 1;
	height = (rect.bottom - rect.top) | 1;
}

void mini3d::OSFunctionW::GetWindowClientSize(const MINI3D_WINDOW windowHandle, unsigned int &width, unsigned int &height) const
{
	RECT rect;
	GetClientRect(windowHandle, &rect);

	// get the width and height (must be bigger than 0)
	width = (rect.right - rect.left) | 1;
	height = (rect.bottom - rect.top) | 1;
}

MINI3D_SCREEN mini3d::OSFunctionW::GetScreenFromWindow(const MINI3D_WINDOW window) const
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	return monitor;
}

// Get screen information
void mini3d::OSFunctionW::GetScreenPosition(const MINI3D_SCREEN screen, unsigned int& x, unsigned int& y) const
{
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(screen, &info);

	x = info.rcMonitor.left;
	y = info.rcMonitor.top;
}
void mini3d::OSFunctionW::GetScreenResolution(const MINI3D_SCREEN screen, unsigned int& width, unsigned int& height ) const
{
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(screen, &info);
		
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);

	width = dm.dmPelsWidth;
	height = dm.dmPelsHeight;
}
unsigned int mini3d::OSFunctionW::GetScreenBitDepth(const MINI3D_SCREEN screen) const
{
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(screen, &info);
		
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);

	return dm.dmBitsPerPel;
}
unsigned int mini3d::OSFunctionW::GetScreenFrequency(const MINI3D_SCREEN screen) const
{
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(screen, &info);
		
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);

	return dm.dmDisplayFrequency;		
}
unsigned int mini3d::OSFunctionW::GetScreenOrientation(const MINI3D_SCREEN screen) const
{
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(screen, &info);
		
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);

	return dm.dmDisplayOrientation;
}
MINI3D_GRAPHICS_CARD mini3d::OSFunctionW::GetGraphicsCardFromScreen(const MINI3D_SCREEN screen) const
{
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(screen, &monitorInfo);
		
	// Enumerate all display adapters.
	DISPLAY_DEVICE device, monitor;
	device.cb = monitor.cb = sizeof(DISPLAY_DEVICE);
		
	DWORD devNum = 0;
	while(EnumDisplayDevices(0, devNum, &device, 0)) 
	{
		// Loop through all monitors this display adapter is attached to (in case it is multi headed).
		DWORD monitorNum = 0;
		while(EnumDisplayDevices(device.DeviceName, monitorNum, &monitor, 0))
		{
			if (monitor.DeviceName == monitorInfo.szDevice)
				return devNum;

			monitorNum++;
		}
		devNum++;
	}
	return 0;
}

// Get Graphics Card Information
int mini3d::OSFunctionW::GetGraphicsCardCount() const
{
	// TODO: This stuff we should really cache and update when there is a screen change!

	// Enumerate all display adapters.
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
		
	int numDevices = 0;

	// Loop through all monitors this display adapter is attached to (in case it is multi headed).
	DWORD deviceNum = 0;
	while(EnumDisplayDevices(0, deviceNum, &device, 0))
	{
		deviceNum++;

		// Skip this device if it is a mirroring device (such as the NetMeetingvdevice) or if it is not attached to the desktop.
		if((device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) || !(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			continue;

		// count this monitor
		numDevices++;
	}

	MINI3D_GRAPHICS_CARD* graphicsCards = new MINI3D_GRAPHICS_CARD[numDevices];

	numDevices = deviceNum = 0;
	while(EnumDisplayDevices(0, deviceNum, &device, 0))
	{
		deviceNum++;

		// Skip this device if it is a mirroring device (such as the NetMeetingvdevice) or if it is not attached to the desktop.
		if((device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) || !(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			continue;

		// count this monitor
		numDevices++;
	}
		
	return numDevices;
}		
	
// Debug function
void mini3d::OSFunctionW::PrintAllGraphicsCardInformation() const
{
	// Enumerate all display adapters.
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
	DWORD devNum = 0;
	while(EnumDisplayDevices(0, devNum, &device, 0)) 
	{
		// Make sure this isn't a mirroring device (such as the NetMeetingvdevice) and is attached to the desktop.
		if(!(device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) && (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) 
		{
			Log("Device Information -----------");
			Log((char*)device.DeviceID);
			Log((char*)"\n");
			Log((char*)device.DeviceKey);
			Log((char*)"\n");
			Log((char*)device.DeviceName);
			Log((char*)"\n");
			Log((char*)device.DeviceString);
			Log((char*)"\n");

			// Loop through all monitors this display adapter is attached to (in case it is multi headed).
			DISPLAY_DEVICE monitor;
			monitor.cb = sizeof(DISPLAY_DEVICE);
			   
			DWORD monitorNum = 0;
			while(EnumDisplayDevices(device.DeviceName, monitorNum, &monitor, 0)) 
			{
				// Make sure this isn't a mirroring device (such as the NetMeeting device) and is attached to the desktop.
				if(!(monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) && (monitor.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) 
				{
				Log("Screen Information -----------");
				Log((char*)monitor.DeviceID);
				Log((char*)"\n");
				Log((char*)monitor.DeviceKey);
				Log((char*)"\n");
				Log((char*)monitor.DeviceName);
				Log((char*)"\n");
				Log((char*)monitor.DeviceString);
				Log((char*)"\n");
				}

				monitorNum += 1;
			}
		}
		devNum += 1;
	}
}

void mini3d::OSFunctionW::SetFullscreenWindow(MINI3D_WINDOW hWindow, const unsigned int& width, const unsigned int& height)
{
	// Get the Device Mode that is closest to the requested resolution
	DEVMODE dm = GetClosestCompatibleGraphicsMode(hWindow, width, height);

	// GetWindow client size
	RECT clientRect;
	GetClientRect((HWND)hWindow, &clientRect);

	int currentWidth = clientRect.right - clientRect.left;
	int currentHeight = clientRect.bottom - clientRect.top;

	// If we are already in fullscreen mode and the requested resolution is the same as the current one, dont set it again.
	if ((fullscreenWindow != 0) && (currentWidth == dm.dmPelsWidth) && (currentHeight == dm.dmPelsHeight))
		return;

	// if we are not already in fullscreen state, capture the original window settings before we change them
	if (fullscreenWindow == 0)
	{
		// Capture window style
		windowStyle = GetWindowLongPtr((HWND)hWindow, GWL_STYLE);
			
		// Capture window position
		GetWindowRect((HWND)hWindow, &winRect);
	}

	// Make the window fullscreen and the same size as the fullscreen desktop
	SetWindowLongPtr((HWND)hWindow, GWL_STYLE, WS_POPUP);
	SetWindowPos((HWND)hWindow, HWND_TOPMOST, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Change Screen Resolution
	if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{      
		// TODO: Handle Error?
		return;
	}

	// Keep track of new fullscreen window
	fullscreenWindow = hWindow;
}
void mini3d::OSFunctionW::RestoreFullscreenWindow(MINI3D_WINDOW hWindow)
{
	// Restore window style
	SetWindowLongPtr((HWND)hWindow, GWL_STYLE, windowStyle);

	// Restore window size and position
	SetWindowPos((HWND)hWindow,
					HWND_TOP,
					winRect.left, 
					winRect.top, 
					winRect.right - winRect.left, 
					winRect.bottom - winRect.top, 
					SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// Restore desktop resolution
	ChangeDisplaySettings(NULL, 0);

	// Set the fullscreenWindow to 0
	fullscreenWindow = 0;		
}

void mini3d::OSFunctionW::Log(char* message) const {};


// ::::: Private Methods ::::::::::::::::::::::::::::::::::::::::::::::::::

GRAPHICS_MODE mini3d::OSFunctionW::GetClosestCompatibleGraphicsMode(MINI3D_WINDOW hWindow, const unsigned int& width, const unsigned int& height)
{
	HMONITOR monitor = GetScreenFromWindow(hWindow);
		
	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(monitor, &info);

	// Get the current device mode
	DEVMODE currentDM;
	currentDM.dmSize = sizeof(currentDM);
	EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &currentDM);

	// If width or height = 0 set them to the current desktop resolution
	if (width == 0 || height == 0)
		return currentDM;

	// initialize the device mode structure for requested device mode
	DEVMODE requestedDM = currentDM;
	requestedDM.dmPelsWidth = width;
	requestedDM.dmPelsHeight = height;

	// initialize the device mode structure for best match
	DEVMODE bestDM = currentDM;

	// Difference in area between best match and 
	unsigned int bestMatchAreaDifference = ScoreGraphicsModeMatch(requestedDM, currentDM); 

	// initialize the device mode structure for looping over all device modes
	DEVMODE dm;
	dm.dmSize = sizeof(dm);

	// loop variable
	unsigned int i = 0;

	// Loop over all display settings and find the best match
	// EnumDisplaySettings returns 0 when we request a displaymode id that is out of range
	while (EnumDisplaySettings(info.szDevice, i++, &dm) != 0)
	{
		// skip modes with wrong color bit depth
		if (dm.dmBitsPerPel != currentDM.dmBitsPerPel)
			continue;

		// skip modes with wrong display orientation
		if (dm.dmOrientation != currentDM.dmOrientation)
			continue;

		unsigned int diff = ScoreGraphicsModeMatch(requestedDM, dm);

		if (diff < bestMatchAreaDifference)
		{
			bestDM = dm;
			bestMatchAreaDifference = diff;
		}
	}
	
	// Return the best match found
	return bestDM;
}
	
unsigned int mini3d::OSFunctionW::ScoreGraphicsModeMatch(const GRAPHICS_MODE dm1, const GRAPHICS_MODE dm2)
{
	// Score the similarity of the display modes by getting the difference between widths, heights and frequencies.
	// We get the total score when we add their absolute values together.
	unsigned int score = (unsigned int)(abs((double)(dm1.dmPelsWidth - dm2.dmPelsWidth)) + 
										abs((double)(dm1.dmPelsHeight - dm2.dmPelsHeight)) + 
										abs((double)(dm1.dmDisplayFrequency - dm2.dmDisplayFrequency)));

	return score;	
}
