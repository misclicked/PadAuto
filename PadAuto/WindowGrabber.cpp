#include "WindowGrabber.h"
#include <string>
#include <cassert>
HBITMAP WindowGrabber::grab(const char* window_name)
{
    RECT rc;
    wchar_t wtext[25];
    mbstowcs_s(nullptr, wtext, strlen(window_name) + 1, window_name, strlen(window_name) + 1);//Plus null
    LPWSTR ptr = wtext;

    HWND hTrayWnd = FindWindow(wtext, NULL);
    HWND hwnd = ::FindWindowEx(0, 0, wtext, 0);    //the window can't be min
    assert(hwnd != NULL);
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    return CreateCompatibleBitmap(hdcScreen,
        rc.right - rc.left, rc.bottom - rc.top);
}
