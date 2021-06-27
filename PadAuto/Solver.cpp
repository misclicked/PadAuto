#include "Solver.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
using namespace std;

const int Solver::dir[8][2] = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{-1,1},{1,-1} };
int conti[8] = {1, 0, 3, 2, 5, 4, 7, 6};

Solver::Solver()
{
	init("");
}

Solver::Solver(string board_string)
{
	init(board_string);
}

void Solver::init(string board_string)
{
	memset(this->board, -1, sizeof(this->board));
	memset(this->visited_board, 0, sizeof(this->visited_board));
	memset(this->count_combo_colors, 0, sizeof(this->count_combo_colors));
	this->input_board_string = board_string;
	int cpos = 0;
	if (this->input_board_string.size() == 30)
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 6; j++) {
				this->board[i][j] = this->input_board_string[cpos++] - '0';
			}
		}
	//Solver::print_board();
}
void Solver::init(vector<unsigned char> pixels, int height, int width)
{
	memset(this->board, -1, sizeof(this->board));
	memset(this->visited_board, 0, sizeof(this->visited_board));
	memset(this->count_combo_colors, 0, sizeof(this->count_combo_colors));
	RGB** rgb = (RGB**)malloc(height * sizeof(RGB*));

	int pixels_counter = 0;
	for (int i = height - 1; i >= 0; i--) {
		rgb[i] = (RGB*)malloc(width * sizeof(RGB));
		for (int j = 0; j < width; j++) {
			int b = pixels[pixels_counter++];
			int g = pixels[pixels_counter++];
			int r = pixels[pixels_counter++];
			pixels_counter++;
			rgb[i][j] = { r, g, b };
		}
	}

	int top_left_x = 595;
	int top_left_y = 3;
	int bot_right_x = 966;
	int bot_right_y = 451;

	int box_size_width = (bot_right_y - top_left_y) / 6;
	int box_size_height = (bot_right_x - top_left_x) / 5;

	int box_size_width_3_1 = box_size_width / 3;
	int box_size_width_3_2 = (box_size_width / 3) * 2;
	int box_size_height_3_1 = box_size_height / 3;
	int box_size_height_3_2 = (box_size_height / 3) * 2;

	RGB board[5][6] = { 0 };

	int x_index = top_left_x + box_size_width_3_1;
	int y_index = 0;
	for (int i = 0; i < 5; i++) {
		x_index += box_size_height;
		y_index = top_left_y + box_size_height_3_1;
		for (int j = 0; j < 6; j++) {
			y_index += box_size_width;
			int counter = 0;
			for (int ii = 0; top_left_x + i * box_size_height + ii < height && ii + box_size_width_3_1 < box_size_width_3_2; ii++) {
				for (int jj = 0; top_left_y + j * box_size_width + jj < width && jj + box_size_height_3_1 < box_size_width_3_2; jj++) {
					board[i][j].R += rgb[x_index + ii][y_index + jj].R;
					board[i][j].G += rgb[x_index + ii][y_index + jj].G;
					board[i][j].B += rgb[x_index + ii][y_index + jj].B;
					counter++;
				}
			}
			board[i][j].R /= counter;
			board[i][j].G /= counter;
			board[i][j].B /= counter;
		}
	}

	string board_string = "";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			int R = board[i][j].R;
			int G = board[i][j].G;
			int B = board[i][j].B;
			if (B > 200) {
				board_string.push_back('1'); //¤ô
			}
			else if (R > 200 && B > 140) {
				board_string.push_back('5'); //¤ß
			}
			else if (R > 200 && G > 200) {
				board_string.push_back('3'); //¥ú
			}
			else if (R > 200) {
				board_string.push_back('0'); //¤õ
			}
			else if (R > 100 && B > 100 && G < 100) {
				board_string.push_back('4'); //·t
			}
			else {
				board_string.push_back('2'); //¤ì
			}
		}
	}
	this->input_board_string = board_string;
	int cpos = 0;
	if (this->input_board_string.size() == 30)
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 6; j++) {
				this->board[i][j] = this->input_board_string[cpos++] - '0';
			}
		}
	//Solver::print_board();
}

void Solver::print_board()
{
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			cout << this->board[i][j];
		}
		cout << endl;
	}
}

void Solver::print_board_()
{
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			cout << this->board_[i][j];
		}
		cout << endl;
	}
}

void Solver::apply_board(pair<int, int> start_pos, vector<int> path)
{
	int curi = start_pos.first, curj = start_pos.second, nexti, nextj;

	for (const auto& it : path) {
		nexti = curi + Solver::dir[it][0];
		nextj = curj + Solver::dir[it][1];
		swap(this->board[curi][curj], this->board[nexti][nextj]);
		curi = nexti, curj = nextj;
	}
}

bool Solver::in_board(int x, int y)
{
	return x >= 1 && x <= 4 && y >= 1 && y <= 6;
}

pair<int, int> Solver::count_combos(int iterator, pair<int, int> start_pos, vector<int> path)
{
	memcpy(this->board_, this->board, sizeof(this->board));

	int curi = start_pos.first, curj = start_pos.second, nexti, nextj;

	for (const auto& it : path) {
		nexti = curi + Solver::dir[it][0];
		nextj = curj + Solver::dir[it][1];
		swap(this->board_[curi][curj], this->board_[nexti][nextj]);
		curi = nexti, curj = nextj;
	}

	int combos = 0;

	int have_combo = 0;
	int link_counter = 1;
	#pragma omp parallel for private(link_counter, have_combo)
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			if (this->visited_board[i][j] == iterator)continue;
			int base = this->board_[i][j];
			if (base == 5)continue;
			// int have_combo = 0;
			// int link_counter = 1;
			have_combo = 0;
			link_counter = 1;
			for (int k = 1; k <= 5; k++) {
				if (this->board_[i][j + k] == base)link_counter++;
				else break;
			}
			if (link_counter >= 3) {
				this->visited_board[i][j] = iterator;
				for (int k = 1; k <= 5; k++) {
					if (this->board_[i][j + k] == base)this->visited_board[i][j + k] = iterator;
					else break;
				}
				have_combo = 1;
			}

			link_counter = 1;
			for (int k = 1; k <= 4; k++) {
				if (this->board_[i + k][j] == base)link_counter++;
				else break;
			}
			if (link_counter >= 3) {
				this->visited_board[i][j] = iterator;
				for (int k = 1; k <= 5; k++) {
					if (this->board_[i + k][j] == base)this->visited_board[i + k][j] = iterator;
					else break;
				}
				have_combo = 1;
			}
			if (have_combo) {
				this->count_combo_colors[base] = iterator;
				#pragma omp atomic
				combos++;
			}
		}
	}

	int five_color = 1;
	for (int i = 0; i < 5; i++) {
		if (this->count_combo_colors[i] != iterator)five_color = 0;
	}

	return { combos, five_color };
}

int Solver::hash_board(pair<int, int> cur_pos, pair<int, int> start_pos, vector<int> path)
{
	memcpy(this->board_, this->board, sizeof(this->board));

	int curi = start_pos.first, curj = start_pos.second, nexti, nextj;

	for (const auto& it : path) {
		nexti = curi + Solver::dir[it][0];
		nextj = curj + Solver::dir[it][1];
		swap(this->board_[curi][curj], this->board_[nexti][nextj]);
		curi = nexti, curj = nextj;
	}

	long long int result = 0, now = 1;
	long long int prime = 1e9 + 7, m = 2e9;

	result += cur_pos.first * now;
	result = result % m;
	now = (now * prime) % m;
	result += cur_pos.second * now;
	result = result % m;
	now = (now * prime) % m;

	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			result += this->board_[i][j] * now;
			result = result % m;
			now = (now * prime) % m;
		}
	}

	return result;
}

Solver::solution Solver::solve(int max_depth)
{
	typedef pair<int, pair<int, int>> pipii;
	int count_colors[6] = { 0 };
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			count_colors[this->board[i][j]]++;
		}
	}
	vector<pipii> vec;
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 6; j++) {
			if (this->board[i][j] == 5) {
				vec.push_back({ INT_MAX, {i, j} });
			}
			else {
				vec.push_back({ count_colors[board[i][j]], {i, j}});
			}
		}
	}
	sort(vec.begin(), vec.end());
	int global_iterator = 1;
	auto comp = [](state a, state b) {
		return a.combo * 3 - a.path.size() < b.combo * 3 - b.path.size();
	};
	priority_queue<state, vector<state>, decltype(comp)> que(comp);
	int current_x, current_y, next_x, next_y;
	unordered_set<long long int> existed_board;
	for (auto const& it : vec) {
		memset(this->visited_board, 0, sizeof(this->visited_board));
		memset(this->count_combo_colors, 0, sizeof(this->count_combo_colors));
		global_iterator = 1;
		int x = it.second.first, y = it.second.second;
		while (que.size())que.pop();
		que.push({{ x, y }, {}, 0});
		while (que.size()) {
			state current_state = que.top();
			que.pop();
			current_x = current_state.cor.first;
			current_y = current_state.cor.second;
			for (int i = 0; i < 8; i++) {
				next_x = current_x + Solver::dir[i][0];
				next_y = current_y + Solver::dir[i][1];
				if (in_board(next_x, next_y)) {
					state next_state = current_state;
					if(next_state.path.size() == 0 || next_state.path.back()!=conti[i]){
						next_state.cor = { next_x, next_y };
						next_state.path.emplace_back(i);
						int board_hash = hash_board(next_state.cor, { x, y }, next_state.path);
						pair<int, int> is_ok = Solver::count_combos(global_iterator++, { x, y }, next_state.path);
						next_state.combo = is_ok.first;
						if (is_ok.second) {
							//cout << "GOOD" << endl;
							//cout << next_state.combo << " " << next_state.path.size() << endl;
							//Solver::apply_board({ x, y }, next_state.path);
							//Solver::print_board();
							return { {x, y}, next_state.path };
						}
						if (existed_board.count(board_hash) || next_state.path.size() > max_depth)continue;
						existed_board.insert(board_hash);
						que.push(next_state);
					}
				}
			}
			if (global_iterator >= 300000)break;
		}
	}
	return solution();
}
