#pragma once
#include <windows.h>
#include <vector>
class WindowGrabber
{
public:
	std::vector<unsigned char> grab(const char* window_name, int& width, int& height);
private:
	std::vector<unsigned char> ToPixels(HBITMAP BitmapHandle, int& width, int& height);
};

