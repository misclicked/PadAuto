#pragma once
#include <utility>
#include <vector>
#include <string>
using namespace std;
class Solver76
{
public:
	Solver76();
	Solver76(string board_string);
	void init(string board_string);
	void init(vector<unsigned char> pixels, int height, int width);
	struct solution {
		pair<int, int> starting_position;
		vector<int> directions;
	};
	const static int dir[8][2];
	int board[15][15];
	Solver76::solution solve(int max_depth);
private:
	void print_board();
	void print_board_();
	void apply_board(pair<int, int> start_pos, vector<int> path);
	int board_[15][15], visited_board[15][15], count_combo_colors[6];
	string input_board_string;
	bool in_board(int x, int y);
	pair<int, int> count_combos(int iterator, pair<int, int> start_pos, vector<int> path);
	int hash_board(pair<int, int> cur_pos, pair<int, int> start_pos, vector<int> path);
	struct state {
		pair<int, int> cor;
		vector<int> path;
		int combo;
	};

	struct RGB {
		int R, G, B;
	};

};

