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

char *dirname[] = {
	"left",
	"left_str_diag",
	"str",
	"right_str_diag",
	"right",
	"right_back_diag",
	"back",
	"left_back_diag",
};

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

void board_init() {
	int i, j, k, idx;
	short col;
	struct coin *cn;
	for(i=0; i<8; i++) {
		if(i<2)
			col = COLOR_WHITE;
		else if(i>5)
			col = COLOR_BLACK;
		else
			col = COLOR_NONE;
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
			cn = &board[i][j];
			cn->color = col;
			cn->x = j;
			cn->y = i;
			if(col == COLOR_BLACK) {
				for(k=0; k<cn->num_dir; k++) {
					cn->x_inc[k] *= -1;
					cn->y_inc[k] *= -1;
				}
			}
		}
	}
}

void board_print() {
	int i, j;
	char disp[] = { 'k','q','b','n','r','p',' ' };
	for(i=7; i>=0; i--) {
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
	printf("%s _________ _________ _________ _________ _________ _________ _________ _________\n", selColorNone);
	for(i=7; i>=0; i--) {
		for(k=0; k<5; k++) {
			if(k==4)
				s='_';
			else
				s=' ';
			for(j=0; j<8; j++) {
				idx = board[i][j].type;
				if(board[i][j].color == COLOR_WHITE)
					col = selColorWhite;
				else if (board[i][j].color == COLOR_BLACK)
					col = selColorBlack;
				else
					col = selColorNone;
//TODO Improve printing of black cells
// Background color '.' can be used to show black cells

				printf("%s|%c%c%5s%c%c", col, s, s, ascii_art[idx][k], s, s);
//				printf("%s|%2d%5s%2d", col, board[i][j].x, ascii_art[idx][k], board[i][j].y);
			}
			printf("|\n");
		}
	}
}

int isOpp(short x, short y) {
	debug ("\t\tAttempting to kill %d %d\n", x, y);
	if(x<0 || y<0 || x>7 || y>7)
		return 0;
	if(board[y][x].color == COLOR_OPP)
		return 1;
	else
		return 0;
}

int isBlank(short x, short y) {
	debug ("\t\tAttempting to reach %d %d\n", x, y);
	if(x<0 || y<0 || x>7 || y>7)
		return 0;
	if(board[y][x].color == COLOR_NONE)
		return 1;
	else
		return 0;
}

int isNotSelf(short x, short y) {
	debug ("\t\tAttempting to move %d %d\n", x, y);
	if(x<0 || y<0 || x>7 || y>7)
		return 0;
	if(board[y][x].color != COLOR_SELF)
		return 1;
	else
		return 0;
}

