
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#include "windows.h"

namespace mini3d
{
namespace utilites
{
class Window
{

public:

	Window(void* WindowCallback, const unsigned int& width, const unsigned int& height)
	{
		memset(&windowClass, 0, sizeof(WNDCLASSEX));
		windowHandle = 0;
		windowHandle = (int)CreateWindowsWindow((WNDPROC)WindowCallback, width, height);
	}

	~Window()
	{
		UnregisterClass("mini3d", GetModuleHandle(NULL));
	}

	int GetWindowHandle() const
	{
		return windowHandle;
	}



private:

	HWND CreateWindowsWindow(WNDPROC wndProc, const unsigned int& width, const unsigned int& height)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wc;

		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = 0;
		wc.lpfnWndProc   = wndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = "mini3D";
		wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}

		windowClass = wc;


		HWND hWindow = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			"mini3D",
			"Mini3D",
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			0, 0, hInstance, 0);

		return hWindow;
	}

	int windowHandle;
	WNDCLASSEX windowClass;
};
}
}
