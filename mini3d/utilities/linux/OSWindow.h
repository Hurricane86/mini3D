
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include<X11/keysym.h>

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
	static WINDOWCALLBACK windowCallback;
	static std::map<int, OSWindow*> windowMap;
	Window window;

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
		window = CreateWin(0, width, height);

		Display *dpy = XOpenDisplay(0);
		XSelectInput(dpy, window, StructureNotifyMask);
	}

	~OSWindow()
	{
		std::map<int, OSWindow*>::iterator iter = windowMap.find((int)windowHandle);
		if (iter != windowMap.end())
		{
			windowMap.erase(iter);
		}
	}

	void Show()
	{
		Display *dpy = XOpenDisplay(0);
		XMapWindow(dpy, window);
	}

	void WaitForMessage()
	{
        for(;;) 
		{
			XEvent e;
			XNextEvent(XOpenDisplay(0), &e);
			if(e.type == XK_Escape)
     		  break; //exit(0);
		}
	}

private:

	// ---------- PRIVATE MEMBER FUNCTIONS ------------------------------------

	Window CreateWin(void* wndProc, const unsigned int& width, const unsigned int& height)
	{
		Display *dpy = XOpenDisplay(0);

		int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
        int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

		return XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, width, height, 0, blackColor, whiteColor);
	}
};
}
}

int mini3d::utilites::OSWindow::width, mini3d::utilites::OSWindow::height;
int mini3d::utilites::OSWindow::x, mini3d::utilites::OSWindow::y;
int mini3d::utilites::OSWindow::mouseX, mini3d::utilites::OSWindow::mouseY, mini3d::utilites::OSWindow::mouseWheelDelta;
int mini3d::utilites::OSWindow::key;	
int mini3d::utilites::OSWindow::windowHandle;
bool mini3d::utilites::OSWindow::leftMouseDown;
mini3d::utilites::OSWindow::WINDOWCALLBACK mini3d::utilites::OSWindow::windowCallback;
std::map<int, mini3d::utilites::OSWindow*> mini3d::utilites::OSWindow::windowMap;

#endif