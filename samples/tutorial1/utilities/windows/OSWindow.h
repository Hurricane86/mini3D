
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "windows.h"
#include <map>

namespace utilities
{
class OSWindow
{

	// ---------- ENUMS -------------------------------------------------------
public:
	enum WindowMessage { SIZE, CLOSED, DESTROYED, MOUSE_MOVE, MOUSE_LEFT_UP, MOUSE_LEFT_DOWN, MOUSE_WHEEL, KEY_DOWN, PAINT };
	enum VirtualKeyCode { VKC_F12 = 0x71 };

private:

	// ---------- TYPEDEFS ----------------------------------------------------
	typedef void (*WINDOWCALLBACK)(OSWindow*, WindowMessage);

	// ---------- MEMBER VARIABLES --------------------------------------------
	static unsigned int clientWidth, clientHeight;
	static int width, height;
	static int x, y;
	static int mouseX, mouseY, mouseWheelDelta;
	static int key;	
	static HWND windowHandle;
	static bool leftMouseDown;
	static WNDCLASSEX windowClass;
	static WINDOWCALLBACK windowCallback;
	static std::map<HWND, OSWindow*> windowMap;
	static OSWindow* defaultWindow;
	
public:

	// ---------- GETTERS AND SETTERS -----------------------------------------

	int GetX() const { return x; };
	int GetY() const { return y; };
	int GetWidth() const { return width; };
	int GetHeight() const { return height; };
	int GetClientWidth() const { return clientWidth; };
	int GetClientHeight() const { return clientHeight; };
	int GetMouseX() const { return mouseX; };
	int GetMouseY() const { return mouseY; };
	bool GetLeftMouseDown() const { return leftMouseDown; };
	int GetMouseWheelDelta() const { return mouseWheelDelta; };
	int GetKey() const { return key; };
	HWND GetWindowHandle() const { return windowHandle; };
	long windowStyle;
	
	// ---------- PUBLIC MEMBER FUNCTIONS -------------------------------------

	OSWindow(WINDOWCALLBACK callback, const unsigned int& width, const unsigned int& height)
	{
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
		windowCallback = callback;
		leftMouseDown = false;
		windowHandle = CreateWindowsWindow(&utilities::OSWindow::WndProc, width, height);
		defaultWindow = this;
	}

	OSWindow(const OSWindow& o) 
	{
		windowHandle = o.windowHandle;
	}

	~OSWindow()
	{
		DestroyWindow((HWND)windowHandle);

		windowMap.erase(windowHandle);

		UnregisterClass(L"mini3d", GetModuleHandle(NULL));		
	}

	static void WaitForMessage()
	{
		MSG msg;
		// Wait for a message to happen
		GetMessage(&msg, NULL, 0, 0);

		// Send the message onwards		
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	void Show()
	{
		ShowWindow((HWND)windowHandle, SW_SHOW);
	}


private:

	// ---------- PRIVATE MEMBER FUNCTIONS ------------------------------------

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
		wc.lpszClassName = L"mini3D";
		wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			// TODO: Error message
		}

		HWND hWindow = CreateWindowEx( WS_EX_APPWINDOW, L"mini3D", L"Mini3D", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
		windowMap[hWindow] = this;

		windowClass = wc;

		return hWindow;
	}

	static void UpdateClientSize()
	{
		LPRECT clientRectangle = new tagRECT();
		GetClientRect(HWND(windowHandle), clientRectangle);

		// get the width and height (must be bigger than 0)
		clientWidth = (clientRectangle->right - clientRectangle->left) | 1;
		clientHeight = (clientRectangle->bottom - clientRectangle->top) | 1;

		delete clientRectangle;
	}

	// callback for our window, handles window messages
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
		std::map<HWND, OSWindow*>::const_iterator iter = windowMap.find(hwnd);
		if (iter == windowMap.end())
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		OSWindow* window = iter->second;

		switch(msg)
		{
			case WM_PAINT:
				windowCallback(window, PAINT);
			break;
			case WM_CLOSE:
				windowCallback(window, CLOSED);
			break;
			case WM_DESTROY:
				windowCallback(window, DESTROYED);
			break;
			case WM_SIZE:
				width = LOWORD(lParam);
				height = HIWORD(lParam);
				UpdateClientSize();				
				windowCallback(window, SIZE);
			break;
			case WM_LBUTTONDOWN:
				leftMouseDown = true;
				SetCapture(hwnd);
				windowCallback(window, MOUSE_LEFT_DOWN);
			break;
			case WM_KEYDOWN:
				key = wParam;
				windowCallback(window, KEY_DOWN);
			break;
			case WM_MOUSEMOVE:
				mouseX = LOWORD(lParam);
				mouseY = HIWORD(lParam);
				windowCallback(window, MOUSE_MOVE);
			break;
			case WM_LBUTTONUP:
				leftMouseDown = false;
				ReleaseCapture();
				windowCallback(window, MOUSE_LEFT_UP);
			break;
			case WM_MOUSEWHEEL:
				mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				windowCallback(window, MOUSE_WHEEL);
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

};
}

std::map<HWND, utilities::OSWindow*> utilities::OSWindow::windowMap;
int utilities::OSWindow::width, utilities::OSWindow::height;
int utilities::OSWindow::x, utilities::OSWindow::y;
unsigned int utilities::OSWindow::clientWidth, utilities::OSWindow::clientHeight;
int utilities::OSWindow::mouseX, utilities::OSWindow::mouseY, utilities::OSWindow::mouseWheelDelta;
int utilities::OSWindow::key;	
HWND utilities::OSWindow::windowHandle;
bool utilities::OSWindow::leftMouseDown;
WNDCLASSEX utilities::OSWindow::windowClass;
utilities::OSWindow::WINDOWCALLBACK utilities::OSWindow::windowCallback;
utilities::OSWindow* utilities::OSWindow::defaultWindow;

#endif