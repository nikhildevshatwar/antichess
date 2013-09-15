#include <stdio.h>
#include "game.h"

#define KNRM	"\x1B[0m"
#define KRED	"\x1B[31m"
#define KGRN	"\x1B[32m"
#define KYEL	"\x1B[33m"
#define KBLU	"\x1B[34m"
#define KMAG	"\x1B[35m"
#define KCYN	"\x1B[36m"
#define KWHT	"\x1B[37m"

char *selColorWhite = KRED;
char *selColorBlack = KCYN;
char *selColorNone = KWHT;

char initial_setup[8][8] = {
	'r','n','b','q','k','b','n','r',
	'p','p','p','p','p','p','p','p',
	' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',
	'p','p','p','p','p','p','p','p',
	'r','n','b','q','k','b','n','r',
};

char ascii_art[][5][6] = {
	{
		' ','_','+','_',' ','\0',
		' ','\\',' ','/',' ','\0',
		' ','(',' ',')',' ','\0',
		' ','/',' ','\\',' ','\0',
		' ','=','=','=',' ','\0',
	},
	{
		' ','x','x','x',' ','\0',
		' ','\\',' ','/',' ','\0',
		' ','(',' ',')',' ','\0',
		' ','/',' ','\\',' ','\0',
		' ','=','=','=',' ','\0',
	},
	{
		' ',' ',' ',' ',' ','\0',
		' ',' ','O',' ',' ','\0',
		' ','(','/',')',' ','\0',
		' ','/',' ','\\',' ','\0',
		' ','=','=','=',' ','\0',
	},
	{
		' ',' ',' ',' ',' ','\0',
		' ',',','^','.',' ','\0',
		'(',' ',' ','\'','\\','\0',
		'|',' ',' ','\\',' ','\0',
		'=','=','=','=',' ','\0',
	},
	{
		' ',' ',' ',' ',' ','\0',
		' ','u','u','u',' ','\0',
		' ','|',' ','|',' ','\0',
		' ','/',' ','\\',' ','\0',
		' ','=','=','=',' ','\0',
	},
   	{
		' ',' ',' ',' ',' ','\0',
		' ',' ','_',' ',' ','\0',
		' ','(',' ',')',' ','\0',
		' ','/',' ','\\',' ','\0',
		' ','=','=','=',' ','\0',
	},
	{
		"     ",
		"     ",
		"     ",
		"     ",
		"_____",
	},
};

struct coin board[8][8];	//current situation - this goes on the stack

struct coin predefined_coins[] = {
	{
		.type = COIN_KING,
		.name = "King",
		.calc_moves = calc_moves_one,
		.isValid = king_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-1, -1, 0, 1, 1, 1, 0, -1},
		.y_inc = {0, 1, 1, 1, 0, -1, -1, -1},
		.num_dir = 8,
	},
	{
		.type = COIN_QUEEN,
		.name = "Queen",
		.calc_moves = calc_moves_multi,
		.isValid = queen_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-1, -1, 0, 1, 1, 1, 0, -1},
		.y_inc = {0, 1, 1, 1, 0, -1, -1, -1},
		.num_dir = 8,
	},
	{
		.type = COIN_BISHOP,
		.name = "Bishop",
		.calc_moves = calc_moves_multi,
		.isValid = bishop_isValidMove,
		.allowed = { left_str_diag, right_str_diag, right_back_diag, left_back_diag },
		.x_inc = {-1, 1, 1, -1},
		.y_inc = {1, 1, -1, -1},
		.num_dir = 4,
	},
	{
		.type = COIN_KNIGHT,
		.name = "Knight",
		.calc_moves = calc_moves_one,
		.isValid = knight_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-2, -2, -1, 1, 2, 2, 1, -1},
		.y_inc = {-1, 1, 2, 2, 1, -1, -2, -2},
		.num_dir = 8,
	},
	{
		.type = COIN_ROOK,
		.name = "Rook",
		.calc_moves = calc_moves_multi,
		.isValid = rook_isValidMove,
		.allowed = { left, str, right, back },
		.x_inc = {-1, 0, 1, 0},
		.y_inc = {0, 1, 0, -1},
		.num_dir = 4,
	},
	{
		.type = COIN_PAWN,
		.name = "Pawn",
		.calc_moves = calc_moves_pawn,
		.isValid = pawn_isValidMove,
		.allowed = { left_str_diag, str, right_str_diag },
		.x_inc = {-1, 0, 1 },
		.y_inc = {1, 1, 1 },
		.num_dir = 3,
	},
	{
		.type = COIN_BLANK,
		.name = "Blank",
		.calc_moves = NULL,
		.isValid = NULL,
		.num_dir = 0,
	},
};

void board_init() {
	int i, j, idx;
	short col;
	for(i=0; i<8; i++) {
		if(i<2)
			col = WHITE;
		else if(i>5)
			col = BLACK;
		else
			col = NONE;
		for(j=0; j<8; j++) {
			switch(initial_setup[i][j]) {
			case 'k':	idx = 0;	break;
			case 'q':	idx = 1;	break;
			case 'b':	idx = 2;	break;
			case 'n':	idx = 3;	break;
			case 'r':	idx = 4;	break;
			case 'p':	idx = 5;	break;
			case ' ':	idx = 6;	break;
			}
			board[i][j] = predefined_coins[idx];
			board[i][j].color = col;
		}
	}
}

void board_print() {
	int i, j;
	char disp[] = { 'k','q','b','n','r','p',' ' };
	for(i=0; i<8; i++) {
		printf("\n+---+---+---+---+---+---+---+---+\n|");
		for(j=0; j<8; j++) {
			printf(" %c +", disp[board[i][j].type]);
		}
	}
	printf("\n+---+---+---+---+---+---+---+---+\n|");
}

void board_print_asciiart() {
	int i, j, k, idx;
	char s;
	char *col;
	for(i=0; i<8; i++) {
		for(k=0; k<5; k++) {
			if(k==4)
				s='_';
			else
				s=' ';
			for(j=0; j<8; j++) {
				idx = board[i][j].type;
				if(board[i][j].color == WHITE)
					col = selColorWhite;
				else if (board[i][j].color == BLACK)
					col = selColorBlack;
				else
					col = selColorNone;
				printf("%s|%c%c%5s%c%c", col, s, s, ascii_art[idx][k], s, s);
			}
			printf("|\n");
		}
	}
}

int isOpp(short x, short y) {
	return 0;
}

int isBlank(short x, short y) {
	return 0;
}

int isNotSelf(short x, short y) {
	return 0;
}

