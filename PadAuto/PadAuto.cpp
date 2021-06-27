// PadAuto.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "Solver.h"
#include <string>
#include <windows.h>
#include "WindowGrabber.h"
#include "MouseMover.h"
using namespace std;
#define OFFSET 96

int main()
{

    HWND window = ::FindWindowEx(0, 0, TEXT("SDL_app"), 0);
    RECT rect = { 0 };
    GetWindowRect(window, &rect);
    MouseMover mouse_mover;
    mouse_mover.rect_bottom = rect.bottom;
    mouse_mover.rect_right = rect.right;

    SetForegroundWindow(window);
    SetActiveWindow(window);
    SetFocus(window);
    Sleep(300);
    while (true)
    {
        WindowGrabber wg;
        Solver solver;
        if (GetKeyState(VK_NUMPAD0) & 0x8000) {
            cout << "key pressed" << endl;
            int height;
            int width;
            vector<unsigned char> pixels = wg.grab("SDL_app", width, height);

            //solver.init("420344302252025230522023111111");
            solver.init(pixels, height, width);
            Solver::solution solution = solver.solve(14);

            cout << "solution ready: steps: " << solution.directions.size() << endl;
            //cin.get();

            mouse_mover.Move(solution);
        }
    }

    return 0;
}



// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
