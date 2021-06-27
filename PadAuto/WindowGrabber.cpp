#include "WindowGrabber.h"
#include <string>
#include <cassert>
std::vector<unsigned char> WindowGrabber::grab(const char* window_name, int& width, int& height)
{
    RECT rc;
    wchar_t wtext[25];
    mbstowcs_s(nullptr, wtext, strlen(window_name) + 1, window_name, strlen(window_name) + 1);//Plus null
    LPWSTR ptr = wtext;

    HWND hwnd = ::FindWindowEx(0, 0, wtext, 0);    //the window can't be min
    assert(hwnd != NULL);
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
        rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //Print to memory hdc
    PrintWindow(hwnd, hdc, PW_CLIENTONLY);

    //copy to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();

    std::vector<unsigned char> result = WindowGrabber::ToPixels(hbmp, width, height);

    //release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);
    return result;
}

std::vector<unsigned char> WindowGrabber::ToPixels(HBITMAP BitmapHandle, int& width, int& height)
{
    BITMAP Bmp = { 0 };
    BITMAPINFO Info = { 0 };
    std::vector<unsigned char> Pixels = std::vector<unsigned char>();

    HDC DC = CreateCompatibleDC(NULL);
    std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
    GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = width = Bmp.bmWidth;
    Info.bmiHeader.biHeight = height = Bmp.bmHeight;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

    Pixels.resize(Info.bmiHeader.biSizeImage);
    GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
    SelectObject(DC, OldBitmap);

    height = std::abs(height);
    DeleteDC(DC);
    return Pixels;
}