#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#define h 22
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
int new_tetromino = 1;
int left_side, right_side, down_side;

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

int main() {
	srand((unsigned int)time(0));
	init_buffer();
	generate_tetromino(5);
	bind_tetromino();
	render();
	int deg = 90;
	Sleep(500);
	rotate_tetromino(deg);
	while (1) {
		detect_collision();
		bind_tetromino();
		render();
		drop_tetromino();
		control();
		Sleep(100);
		if (drop == 0) generate_tetromino(1);
	}
	ch = _getch();
	return 0;
}



void detect_collision() {
	// left wall
	if (left_side + x_off <= 1) {
		slide_left = 0;
	}
	else {
		slide_left = -1;
	}

	// right wall
	if (right_side + x_off >= w - 2) {
		slide_right = 0;
	}
	else {
		slide_right = 1;
	}

	// tetromino-tetromino
	unbind_tetromino();
	for (int j = 0; j < 4; j++) {
		for (int i = 3; i >= 0; i--) {
			if (tetromino[i][j] == 'X' && buffer[i + y_off + 1][j + x_off] == 'X') drop = 0;
		}
	
		if (tetromino[right_side][j] == 'X' && buffer[right_side + x_off + 1][j] == 'X') slide_right = 0;
		if (tetromino[left_side][j] == 'X' && buffer[left_side + x_off - 1][j] == 'X') slide_left = 0;

		//if (tetromino[down_side][j] == 'X' && buffer[down_side + y_off + 1][j + x_off] == 'X') {
			//drop = 0;
			//slide_left = 0; slide_right = 0;
	//	}
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
	new_tetromino = 1;
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
}

void rotate_tetromino(int angle) {
	string s = current_tetromino;
	int index = 0;
	unbind_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					tetromino[i][j] = s[index];
					if (angle == 90) {
						index = 12 - 4 * j + i;
						tetromino[i][j] = s[index];
					}
					if (angle == 180) {
						index = 15 - j - 4 * i;
						tetromino[i][j] = s[index];
					}
					if (angle == 270) {
						index = 3 + 4 * j - i;
						tetromino[i][j] = s[index];
					}
					if (angle == 360) {
						index = j + 4 * i;
						tetromino[i][j] = s[index];
					}
				}
			}
	calculate_sides();
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