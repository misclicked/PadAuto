#pragma once
#include <vector>
#include "Solver76.h"
class MouseMover76
{
public:
	int rect_right;
	int rect_bottom;
	void Move(Solver76::solution solution);
	void MoveOnce(int y, int x, int direction, int first = 0);
};

