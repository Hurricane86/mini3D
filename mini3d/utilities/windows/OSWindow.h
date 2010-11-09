
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef _WIN32

#include "windows.h"
#include <map>

namespace mini3d
{
namespace utilites
{
class OSWindow
{

	// ---------- ENUMS -------------------------------------------------------
public:
	enum WindowMessage { SIZE, CLOSED, DESTROYED, MOUSE_MOVE, MOUSE_LEFT_UP, MOUSE_LEFT_DOWN, MOUSE_WHEEL, KEY_DOWN, PAINT };

private:
	// ---------- FORWARD DECLARATIONS ----------------------------------------
//	WNDPROC WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ---------- TYPEDEFS ----------------------------------------------------
	typedef void (*WINDOWCALLBACK)(OSWindow*, WindowMessage);

	// ---------- MEMBER VARIABLES --------------------------------------------
	static int width, height;
	static int x, y;
	static int mouseX, mouseY, mouseWheelDelta;
	static int key;	
	static int windowHandle;
	static bool leftMouseDown;
	static WNDCLASSEX windowClass;
	static WINDOWCALLBACK windowCallback;
	static std::map<int, OSWindow*> windowMap;
	static OSWindow* defaultWindow;

public:

	// ---------- GETTERS AND SETTERS -----------------------------------------

	int GetX() const { return x; };
	int GetY() const { return y; };
	int GetWidth() const { return width; };
	int GetHeight() const { return height; };
	int GetMouseX() const { return mouseX; };
	int GetMouseY() const { return mouseY; };
	bool GetLeftMouseDown() const { return leftMouseDown; };
	int GetMouseWheelDelta() const { return mouseWheelDelta; };
	int GetKey() const { return key; };
	int GetWindowHandle() const	{ return windowHandle; };

	
	// ---------- PUBLIC MEMBER FUNCTIONS -------------------------------------

	OSWindow(WINDOWCALLBACK callback, const unsigned int& width, const unsigned int& height)
	{
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
		windowCallback = callback;
		leftMouseDown = false;
		windowHandle = (int)CreateWindowsWindow(&mini3d::utilites::OSWindow::WndProc, width, height);
		defaultWindow = this;
	}

	OSWindow(const OSWindow& o) 
	{
		windowHandle = o.windowHandle;
	}

	void doCleanup()
	{

	}
	~OSWindow()
	{
		DestroyWindow((HWND)windowHandle);

		windowMap.erase(windowHandle);

		UnregisterClass(L"mini3d", GetModuleHandle(NULL));		
	}

	void WaitForMessage()
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

		HWND hWindow = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, L"mini3D", L"Mini3D", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
		windowMap[(int)hWindow] = this;

		windowClass = wc;

		return hWindow;
	}

	// callback for our window, handles window messages
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
		std::map<int, OSWindow*>::const_iterator iter = windowMap.find((int)hwnd);
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
				//windowCallback(window, SIZE);
			break;
			case WM_LBUTTONDOWN:
				leftMouseDown = true;
				SetCapture(hwnd);
				windowCallback(window, MOUSE_LEFT_DOWN);
			break;
			case WM_KEYDOWN:
				key = wParam;
				windowCallback(window, MOUSE_LEFT_DOWN);
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
}

std::map<int, mini3d::utilites::OSWindow*> mini3d::utilites::OSWindow::windowMap;
int mini3d::utilites::OSWindow::width, mini3d::utilites::OSWindow::height;
int mini3d::utilites::OSWindow::x, mini3d::utilites::OSWindow::y;
int mini3d::utilites::OSWindow::mouseX, mini3d::utilites::OSWindow::mouseY, mini3d::utilites::OSWindow::mouseWheelDelta;
int mini3d::utilites::OSWindow::key;	
int mini3d::utilites::OSWindow::windowHandle;
bool mini3d::utilites::OSWindow::leftMouseDown;
WNDCLASSEX mini3d::utilites::OSWindow::windowClass;
mini3d::utilites::OSWindow::WINDOWCALLBACK mini3d::utilites::OSWindow::windowCallback;
mini3d::utilites::OSWindow* mini3d::utilites::OSWindow::defaultWindow;

#endif