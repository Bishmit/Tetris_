#include <iostream>
#include <conio.h>
#include <windows.h>
#define h 23
#define w 12
using namespace std;

struct tetromino {
	string t = ".X..XXX.........";
	string o = "XX..XX..........";
	string l = ".X...X...XX.....";
	string j = ".X...X..XX......";
	string i = "X...X...X...X...";
	string s = ".XX.XX..........";
	string z = "XX...XX.........";
};

struct tetromino tet;
string current_tetromino;
char ch;
char buffer[h][w];
char tetromino[4][4];
int score = 0;
int game_over = 0;
int drop = 1;
int x_off = 4, y_off = 4;
int slide_left = -1, slide_right = 1;

void init_buffer();
void render();
void parse_pattern(string);
void generate_tetromino(int);
void bind_tetromino();
void clear_tetromino();
void rotate_tetromino(int);
void drop_tetromino();
void detect_collision();
void control();

int main() {
	init_buffer();
	generate_tetromino(6);
	bind_tetromino();
	render();
	int deg = 90;
	Sleep(500);
	rotate_tetromino(deg);
	while (1) {
		bind_tetromino();
		render();
		detect_collision();
		drop_tetromino();
		control();
		Sleep(100);
	}
	ch = _getch();
	return 0;
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

void drop_tetromino() {
	clear_tetromino();
	y_off += drop;
}

void detect_collision() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// ground collision
			if (i + y_off + 1 == h - 1 && tetromino[i][j] == 'X') drop = 0;
			// left wall collision
			if (j + x_off - 1 < 1 && tetromino[i][j] == 'X') slide_left = 0;
			// right wall collision
			if (j + x_off + 1 > w - 2 && tetromino[i][j] == 'X') slide_right = 0;
		}
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

void generate_tetromino(int n) {
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
}

void bind_tetromino() {
	clear_tetromino();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if(tetromino[i][j]=='X')
			buffer[i + y_off][j + x_off] = tetromino[i][j];
		}
	}
}

void clear_tetromino() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetromino[i][j] == 'X')
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

void rotate_tetromino(int angle) {
	string s = current_tetromino;
	int index;
	if (current_tetromino != tet.o) {
		switch (angle) {
		case 90:
			clear_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					index = 12 - 4 * j + i;
					tetromino[i][j] = s[index];
				}
			}
			break;
		case 180:
			clear_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					index = 15 - j - 4 * i;
					tetromino[i][j] = s[index];
				}
			}
			break;
		case 270:
			clear_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					index = 3 + 4 * j - i;
					tetromino[i][j] = s[index];
				}
			}
			break;
		case 360:
			clear_tetromino();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					index = j + 4 * i;
					tetromino[i][j] = s[index];
				}
			}
			break;
		}
	}
	
}