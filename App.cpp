#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#define h 30
#define w 12
using namespace std;

struct tetromino {
	string t = "....XXX..X......";
	string o = ".....XX..XX.....";
	string l = ".X...X...XX.....";
	string j = "..X...X..XX.....";
	string i = ".X...X...X...X..";
	string s = ".....XX.XX......";
	string z = "....XX...XX.....";
};

struct tetromino tet;
string current_tetromino;
char ch;
char buffer[h][w];
char tetromino[5][4];
int score = 0;
int game_over = 0;
int drop = 1;
int x_off = 4, y_off = 1;
int slide_left = -1, slide_right = 1;
int left_side, right_side, down_side;
int rotation_dir = 0;
char play = 'y';

void init_buffer();
void render();
void parse_pattern(string);
void generate_tetromino(int);
void bind_tetromino();
void unbind_tetromino();
void rotate_tetromino(int);
void drop_tetromino();
void detect_collision();
void control();
void calculate_sides();
void check_line();
void game_over_message();
void reset_game();
void home_screen();

int main() {
	srand((unsigned int)time(0));
	home_screen();
	while (play == 'y') {
		init_buffer();
		reset_game();
		while (!game_over) {
			rotate_tetromino(rotation_dir);
			detect_collision();
			bind_tetromino();
			render();
			check_line();
			drop_tetromino();
			control();
			Sleep(100);
			if (drop == 0) generate_tetromino(1 + rand() % 7);
		}
		game_over_message();
	}
	return 0;
}

void home_screen() {
	cout << "a: left  d: right  w: rotate clockwise  s: rotate anti_clockwise" << endl;
	cout << "Press any key to start...";
	play = _getch();
	play = 'y';
}

void reset_game() {
	score = 0;
	game_over = 0;
	drop = 1;
	x_off = 4; y_off = 1;
	slide_left = -1; slide_right = 1;
	rotation_dir = 0;
	//generate_tetromino(1 + rand() % 7);
	generate_tetromino(5);
	bind_tetromino();
	render();
	Sleep(500);
}
void game_over_message() {
	cout << "Game Over!" << endl << "Play again(y)? ";
	cin >> play;
}

void check_line() {
	int n = 0;
	for (int i = h - 2; i >= 1; i--) {
		n = 0;
		for (int j = 1; j < w - 1; j++) {
			if (buffer[i][j] == 'X') n++;
			else break;
		}
		if (n == 10) {
			score++;
			Beep(500, 100);
			unbind_tetromino();
			for (int k = i; k > 1; k--) {
				for (int j = 1; j < w - 1; j++) {
					buffer[k][j] = buffer[k - 1][j];
				}
			}
			bind_tetromino();
		}
	}
}

void detect_collision() {
	//left wall
	if (left_side + x_off <= 0) {
		slide_left = 0;
		x_off = 4; 
	}
	else {
		slide_left = -1;
	}

	// right wall
	if (right_side + x_off >= w - 1) {
		slide_right = 0; 
		x_off = w-5;
	}
	else {
		slide_right = 1;
	}

	// tetromino-tetromino
	unbind_tetromino();
	for (int j = 0; j < 4; j++) {
		for (int i = 3; i >= 0; i--) {
			if (tetromino[i][j] == 'X' && buffer[i + y_off + 1][j + x_off] == 'X') {
				drop = 0;
				slide_left = 0;
				slide_right = 0;
			}
		}
	
		if (tetromino[j][right_side] == 'X' && buffer[j + y_off][right_side + x_off + 1] == 'X') slide_right = 0;
		if (tetromino[j][left_side] == 'X' && buffer[j + y_off][left_side + x_off - 1] == 'X') slide_left = 0;
	}

	for (int j = 1; j < w - 1; j++) {
		if (buffer[5][j] == 'X') game_over = 1;
	}

	bind_tetromino();
	
	// ground
	if (down_side + y_off == h - 2) {
		drop = 0;
		slide_left = 0; slide_right = 0;
	}
}

void control() {
	char c;
	if (_kbhit()) {
		c = _getch();
		switch (c) {
		case 'a':
			x_off += slide_left;
			break;
		case 'd':
			x_off += slide_right;
			break;
		case 'w':
			rotation_dir = (rotation_dir + 1) % 4;
			break;
		case 's':
			rotation_dir = (rotation_dir - 1 + 4) % 4;
			break;
		}
		bind_tetromino();
	}
}

void bind_tetromino() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetromino[i][j] == 'X' && i + y_off > 0 && i + y_off < h - 1 && j + x_off > 0 && j + x_off < w - 1) {
				buffer[i + y_off][j + x_off] = tetromino[i][j];
			}
		}
	}
}

void unbind_tetromino() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetromino[i][j] == 'X' && j + x_off != w-1 && j + x_off != 0)
				buffer[i + y_off][j + x_off] = ' ';
		}
	}
}

void parse_pattern(string s) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tetromino[i][j] = s[4 * i + j];
		}
	}
}

void generate_tetromino(int n) {
	Sleep(100);
	slide_left = -1; slide_right = 1;
	x_off = 4, y_off = 1;
	drop = 1;
	switch (n) {
	case 1:
		// T
		parse_pattern(tet.t);
		current_tetromino = tet.t;
		break;
	case 2:
		// J
		parse_pattern(tet.j);
		current_tetromino = tet.j;
		break;
	case 3:
		// L
		parse_pattern(tet.l);
		current_tetromino = tet.l;
		break;
	case 4: 
		// O
		parse_pattern(tet.o);
		current_tetromino = tet.o;
		break;
	case 5:
		// I
		parse_pattern(tet.i);
		current_tetromino = tet.i;
		break;
	case 6:
		// S
		parse_pattern(tet.s);
		current_tetromino = tet.s;
		break;
	case 7:
		// Z
		parse_pattern(tet.z);
		current_tetromino = tet.z;
		break;
	}
	calculate_sides();
}

void drop_tetromino() {
	if (drop) {
		unbind_tetromino();
		y_off += drop;
	}
}

void init_buffer() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			buffer[i][j] = ' ';
			if (i == 0 || i == h - 1 || j == 0 || j == w - 1) buffer[i][j] = 'X';
		}
	}
}
void render() {
	system("cls");
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			cout << buffer[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Score: " << score << endl;
}

void rotate_tetromino(int angle) {
	string s = current_tetromino;
	int index = 0;
	unbind_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					tetromino[i][j] = s[index];
					if (angle == 1) {
						index = 12 - 4 * j + i;
						tetromino[i][j] = s[index];
					}
					if (angle == 2) {
						index = 15 - j - 4 * i;
						tetromino[i][j] = s[index];
					}
					if (angle == 3) {
						index = 3 + 4 * j - i;
						tetromino[i][j] = s[index];
					}
					if (angle == 0) {
						index = j + 4 * i;
						tetromino[i][j] = s[index];
					}
				}
			}
	calculate_sides();
	if (right_side + x_off >= w - 1) x_off -= right_side - (w - 2);
	if (left_side + x_off <= 0) x_off += (1 - left_side);
	for (int i = down_side; i >= 0; i--) {
		for (int j = left_side; j <= right_side ; j++) {
			if (tetromino[i][j] == 'X' && buffer[i + y_off][j + x_off] == 'X') y_off--;
		}
	}
	bind_tetromino();
}

void calculate_sides() {
	for (int j = 3; j >= 0; j--) {
		for (int i = 0; i < 4; i++) {
			if (tetromino[i][j] == 'X') left_side = j;
		}
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (tetromino[i][j] == 'X') right_side = j;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetromino[i][j] == 'X') down_side = i;
		}
	}
}
