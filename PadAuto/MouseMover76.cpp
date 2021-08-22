#include "MouseMover76.h"
#include <windows.h>

void MouseMover76::Move(Solver76::solution solution)
{
    int top_left_x = this->rect_right - 424;
    int top_left_y = this->rect_bottom - 357;


    int box_size_width = 63;
    int box_size_height = 63;
    int bot_right_x = top_left_x + box_size_width * 6;
    int bot_right_y = top_left_y + box_size_height * 5;

    int nowx = solution.starting_position.first;
    int nowy = solution.starting_position.second;
    int first = 1;
    vector<pair<int, int>> vec;
    for (const auto& it : solution.directions) {
        first = 0;
        vec.push_back({ top_left_x + box_size_width * (nowy - 1), top_left_y + box_size_height * (nowx - 1) });
        nowx += Solver76::dir[it][0];
        nowy += Solver76::dir[it][1];
    }
    vec.push_back({ top_left_x + box_size_width * (nowy - 1), top_left_y + box_size_height * (nowx - 1) });
    SetCursorPos(vec[0].first, vec[0].second);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    for (const auto& it : vec) {
        SetCursorPos(it.first, it.second);
        Sleep(45);
    }
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void MouseMover76::MoveOnce(int y, int x, int direction, int first)
{
    int top_left_x = this->rect_right - 424;
    int top_left_y = this->rect_bottom - 357;
    //int bot_right_x = this->rect_right - 47;
    //int bot_right_y = this->rect_bottom - 60;

    int box_size_width = 63;
    int box_size_height = 63;
    int bot_right_x = top_left_x + box_size_width * 6;
    int bot_right_y = top_left_y + box_size_height * 5;

    int srcx = top_left_x + box_size_width * (x - 1);
    int srcy = top_left_y + box_size_height * (y - 1);

    int desx = srcx + box_size_width * Solver76::dir[direction][1];
    int dexy = srcy + box_size_height * Solver76::dir[direction][0];

    SetCursorPos(srcx, srcy);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    if (first)Sleep(100);
    for (int i = 0; i <= 2; i++) {
        Sleep(50);
        SetCursorPos(srcx + ((desx - srcx) / 2) * i, srcy + ((dexy - srcy) / 2) * i);
    }
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
