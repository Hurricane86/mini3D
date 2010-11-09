
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifdef __linux

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include<X11/keysym.h>

#include <map>
#include <math.h>

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
		XDestroyWindow(dpy, window);
	}

	void Show()
	{
		XMapWindow(dpy, window);
	}

	static void WaitForMessage()
	{
		Atom wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		
        for(;;) 
		{
			XEvent e;
			XNextEvent(dpy, &e);
			switch (e.type)
			{
				case Expose:
					if (e.xexpose.count == 0)
                    {
						windowCallback(this, PAINT);
					}
				break;
				case ClientMessage:
					if (e.xclient.data.l[0] == wmDeleteMessage)
					{
						DestoryWindow();				
						windowCallback(this, CLOSED);
					}
				break;
				case DestroyNotify:
					windowCallback(this, DESTROYED);
				break;
				case ConfigureNotify:
					if (e.xconfigure.width != width || e.xconfigure.height != height || e.xconfigure.x != x || e.xconfigure.y != y)
					{
						x = e.xconfigure.x;
						y = e.xconfigure.y;

						width = e.xconfigure.width;
						height = e.xconfigure.height;

						windowCallback(this, SIZE);
					}
				break;
				case ButtonPress:
					if (e.xbutton.button == Button1)
					{
						leftMouseDown = true;
						windowCallback(this, MOUSE_LEFT_DOWN);
					}
				break;
				case ButtonRelease:
					if (e.xbutton.button == Button1)
					{
						leftMouseDown = false;
						mouseX = e.xmotion.x;
						mouseY = e.xmotion.y;
						windowCallback(this, MOUSE_MOVE);
						windowCallback(this, MOUSE_LEFT_UP);
					}
					else if (e.xbutton.button == Button4)
					{
						mouseWheelDelta = 50;
						windowCallback(this, MOUSE_WHEEL);						
					}
					else if (e.xbutton.button == Button5)
					{
						mouseWheelDelta = -50;
						windowCallback(this, MOUSE_WHEEL);
					}
					break;
				case MotionNotify:
					mouseX = e.xmotion.x;
					mouseY = e.xmotion.y;

					XEvent ev;
					while (XCheckTypedEvent(dpy, MotionNotify, &ev))
					{
						mouseX = ev.xmotion.x;
						mouseY = ev.xmotion.y;
					}
					windowCallback(this, MOUSE_MOVE);
				break;
				case KeyPress:
					if (XLookupKeysym(&e.xkey, 0) == XK_Escape)
						windowCallback(this, MOUSE_LEFT_DOWN);
				break;
			}
		}
		//windowCallback(this, PAINT);
	}

private:

	// ---------- PRIVATE MEMBER FUNCTIONS ------------------------------------

	Window CreateWin(void* wndProc, const unsigned int& width, const unsigned int& height)
	{
		dpy = XOpenDisplay(0);
		
		int x = 0;
		int y = 0;

		int s = DefaultScreen(dpy);
		int blackColor = BlackPixel(dpy, s);
        int whiteColor = WhitePixel(dpy, s);
		Window w = XCreateSimpleWindow(dpy, RootWindow(dpy, s), x, y, width, height, 0, blackColor, whiteColor);
		XFlush(dpy);
		XSelectInput (dpy, w, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask );
		return w;
	}
	
};
}

int mini3d::utilites::OSWindow::width, mini3d::utilites::OSWindow::height;
int mini3d::utilites::OSWindow::x, mini3d::utilites::OSWindow::y;
int mini3d::utilites::OSWindow::mouseX, mini3d::utilites::OSWindow::mouseY, mini3d::utilites::OSWindow::mouseWheelDelta;
int mini3d::utilites::OSWindow::key;	
int mini3d::utilites::OSWindow::windowHandle;
bool mini3d::utilites::OSWindow::leftMouseDown;
mini3d::utilites::OSWindow::WINDOWCALLBACK mini3d::utilites::OSWindow::windowCallback;

#endif