// PadAuto.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "Solver.h"
#include <string>
#include <windows.h>
#include "WindowGrabber.h"
#include "MouseMover.h"
using namespace std;
#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3
#define RDOWN 4
#define LUP 5
#define RUP 6
#define LDOWN 7

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
        if (GetKeyState(VK_F3) & 0x8000) {
            cout << "key pressed" << endl;
            int height;
            int width;
            vector<unsigned char> pixels = wg.grab("SDL_app", width, height);

            //solver.init("420344302252025230522023111111");

            solver.init(pixels, height, width);

            int target_board_tmp[15][15];
            memset(target_board_tmp, -1, sizeof(target_board_tmp));
            for (int i = 1; i <= 6; i++) {
                target_board_tmp[5][i] = 3;
            }

            //暗貫通
            for (int i = 1; i <= 3; i++) {
                for (int j = 2; j <= 4; j++) {
                    target_board_tmp[j][i] = 4;
                }
            }

            vector<int> vec{ 0,0,0,0,0, 0};
            //光位置0上1下
            //暗位置0左上1右上2左下3右下4中左5中右
            //2way位置0左1右
            //2way位置0上1上中2下中3下
            //2way顏色
            //十字顏色
            Solver::solution bestsolu;
            int bestStep = INT_MAX;
            for (int i = 0; i < 2; i++) { //光橫
                int lightTop = (i == 0) ? 1 : 5;
                for (int j = 0; j < 6; j++) { //暗貫通
                    if ((j / 2) == i)continue;
                    pair<int,int> darkStart;
                    if (j == 0)darkStart = { 1,1 };
                    if (j == 1)darkStart = { 1,4 };
                    if (j == 2)darkStart = { 3,1 };
                    if (j == 3)darkStart = { 3,4 };
                    if (j == 4)darkStart = { 2,1 };
                    if (j == 5)darkStart = { 2,4 };
                    for (int k = 0; k < 2; k++) { //2way second 左右
                        if ((j & 1) != (k & 1))continue;
                        pair<int, int> two_way_start;
                        if (k == 0)two_way_start.second = 1;
                        else two_way_start.second = 3;
                        for (int a = 0; a < 4; a++) { //2way first 上中下
                             if (i == 0 && a == 0)continue;
                            if (i == 1 && a == 3)continue;
                            if (j >= 4 && ((a >= 1) && (a<=2)))continue;
                            if ((j / 2) * 2 == (a/2)*2)continue;
                            if (a == 0) two_way_start.first = 1;
                            if (a == 1) two_way_start.first = 2;
                            if (a == 2) two_way_start.first = 4;
                            if (a == 3) two_way_start.first = 5;
                            for (int b = 0; b < 3; b++) {
                                for (int c = 0; c < 3; c++) {
                                    if (b == c)continue;
                                    for (int d = 0; d < 3; d++) {
                                        if (d == c || d == b)continue;
                                        memset(target_board_tmp, -1, sizeof(target_board_tmp));
                                        //光橫
                                        for (int ii = 1; ii <= 6; ii++) {
                                            target_board_tmp[lightTop][ii] = 3;
                                        }
                                        //暗貫通
                                        for (int ii = darkStart.first; ii <= darkStart.first + 2; ii++) {
                                            for (int jj = darkStart.second; jj <= darkStart.second + 2; jj++) {
                                                target_board_tmp[ii][jj] = 4;
                                            }
                                        }
                                        //2way
                                        for (int ii = two_way_start.second; ii <= two_way_start.second+3; ii++) {
                                            target_board_tmp[two_way_start.first][ii] = b;
                                        }
                                        //十字 L 左右
                                        pair<int, int> lastPos;
                                        int lastType;
                                        /***
                                        * lastType: 1
                                        * X11
                                        * X21
                                        * 222
                                        * 121
                                        * lastType: 2
                                        * 11X
                                        * 12X
                                        * 222
                                        * 121
                                        * lastType: 3
                                        * X11
                                        * 121
                                        * 222
                                        * X21
                                        * lastType: 4
                                        * 11X
                                        * 121
                                        * 222
                                        * 12X
                                        ***/
                                        //暗位置0左上1右上2左下3右下4中左5中右 j
                                        if (k == 0) { //2way在左
                                            if (a == 0) { //2way在上
                                                lastPos = { 1, 4 };
                                                lastType = 1;
                                            }
                                            else if (a == 1) { //2way在上中
                                                lastPos = { 2, 4 };
                                                lastType = 1;
                                            }
                                            else if (a == 2) { //2way在下中
                                                lastPos = { 1, 4 };
                                                lastType = 3;
                                            }
                                            else if (a == 3) { //2way在下
                                                lastPos = { 2, 4 };
                                                lastType = 3;
                                            }
                                        }else {
                                            if (a == 0) { //2way在上
                                                lastPos = { 1, 1 };
                                                lastType = 2;
                                            }
                                            else if (a == 1) { //2way在上中
                                                lastPos = { 2, 1 };
                                                lastType = 2;

                                            }
                                            else if (a == 2) { //2way在下中
                                                lastPos = { 1, 1 };
                                                lastType = 4;

                                            }
                                            else if (a == 3) { //2way在下
                                                lastPos = { 2, 1 };
                                                lastType = 4;

                                            }
                                        }

                                        if (lastType == 1) {
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 0] = b;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 0] = b;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 1] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 2] = c;

                                        }else if (lastType == 2) {
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 2] = b;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 2] = b;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 1] = c;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 0] = c;

                                        }else if (lastType == 3) {
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 0] = b;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 0] = b;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 1] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 2] = c;

                                        }else if (lastType == 4) {
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 2] = b;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 2] = b;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 1][lastPos.second + 2] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 0] = c;
                                            target_board_tmp[lastPos.first + 0][lastPos.second + 1] = c;
                                            target_board_tmp[lastPos.first + 3][lastPos.second + 0] = c;

                                        }

                                        //十字
                                        target_board_tmp[lastPos.first + 1][lastPos.second + 1] = d;
                                        target_board_tmp[lastPos.first + 2][lastPos.second + 0] = d;
                                        target_board_tmp[lastPos.first + 2][lastPos.second + 1] = d;
                                        target_board_tmp[lastPos.first + 2][lastPos.second + 2] = d;
                                        target_board_tmp[lastPos.first + 3][lastPos.second + 1] = d;
                                        solver.backup_board();
                                        Solver::solution solution = solver.solvePlain(target_board_tmp);
                                        solver.resume_board();
                                        if (bestStep > solution.directions.size()) {
                                            bestStep = solution.directions.size();
                                            bestsolu = solution;
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
            }

            mouse_mover.Move(bestsolu);
        }
        if (GetKeyState(VK_F4) & 0x8000) {
            cout << "key pressed" << endl;
            int height;
            int width;
            vector<unsigned char> pixels = wg.grab("SDL_app", width, height);

            //solver.init("420344302252025230522023111111");

            solver.init(pixels, height, width);

            int target_board_tmp[15][15] = 
            {
                { -1, -1, -1, -1, -1, -1, -1, -1, -1},
                { -1, 0, 1, 2, 4, 4, 4, -1, -1},
                { -1, 0, 1, 2, 0, 1, 2, -1, -1},
                { -1, 0, 1, 2, 4, 4, 4, -1, -1},
                { -1, 0, 1, 2, 3, 3, 3, -1, -1},
                { -1, 3, 3, 3, 4, 4, 4, -1, -1},
                { -1, -1, -1, -1, -1, -1, -1, -1, -1},
            };


            Solver::solution solution = solver.solvePlain(target_board_tmp);
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
