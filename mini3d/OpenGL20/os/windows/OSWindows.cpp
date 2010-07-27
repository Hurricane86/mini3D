#include "OSWindows.h"
#include <Windows.h>

unsigned int mini3d::OSWindows::GetMonitorBitDepth() const
{
	// read the monitor information from win32
	DEVMODE devMode;
	memset(&devMode, 0, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);

	// get monitor settings from os
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);

	switch(devMode.dmBitsPerPel)
	{
		case 16:
			return 16;
		case 32:
			return 32;
	}

	// Default case for when running 8 strange desktop modes. (I have no idea what mode that would be!)
	return 16;
}
