#pragma once
#include <vector>
#include "Solver.h"
class MouseMover
{
public:
	int rect_right;
	int rect_bottom;
	void Move(Solver::solution solution);
	void MoveOnce(int y, int x, int direction, int first = 0);
};

