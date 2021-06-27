#pragma once
#include <windows.h>
class WindowGrabber
{
public:
	HBITMAP grab(const char* window_name);
};

