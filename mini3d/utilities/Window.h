/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

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
