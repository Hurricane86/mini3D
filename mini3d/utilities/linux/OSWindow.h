
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
	Display *dpy;

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
	int GetWindowHandle() const	{ return window; };

	
	// ---------- PUBLIC MEMBER FUNCTIONS -------------------------------------

	OSWindow(WINDOWCALLBACK callback, const unsigned int& width, const unsigned int& height)
	{
		windowCallback = callback;
		window = CreateWin(0, width, height);
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
		XMapWindow(dpy, window);
	}

	void WaitForMessage()
	{
        for(;;) 
		{
			XEvent e;
			XNextEvent(dpy, &e);
			if(e.type == KeyPress)
			{
     		  if (XLookupKeysym(&e.xkey, 0) == XK_Escape)
			  {
				break;
			  }
			}
			windowCallback(this, PAINT);
		}
	}

private:

	// ---------- PRIVATE MEMBER FUNCTIONS ------------------------------------

	Window CreateWin(void* wndProc, const unsigned int& width, const unsigned int& height)
	{
		dpy = XOpenDisplay(0);

		int s = DefaultScreen(dpy);
		int blackColor = BlackPixel(dpy, s);
        int whiteColor = WhitePixel(dpy, s);
		Window w = XCreateSimpleWindow(dpy, RootWindow(dpy, s), 0, 0, width, height, 0, blackColor, whiteColor);
		XFlush(dpy);
		XSelectInput (dpy, w, ExposureMask | KeyPressMask | ButtonPressMask);
		return w;
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