#include <stdio.h>
#include "game.h"
/*

Ideas
- save all the logs for every game
- allow to feed a text file as the initial moves
- debug traces
- use threads at first level to solve generate tree parallely
- use timers and tease the player if taking too much time
- use flags where you don't have to calculate something again
- optimize searching for more possiblities if at least one killing is possible
- test by running it againt itself
- when confused, choose random result and debug print the situation
- You can show what all moves user can play if killing is necessory
- When you are waiting for user input, build the tree more and more (if defined aggressive)

Calculating the coeffs intelligently
- Highest = max level when user never gets a chance to play

opt1 - make board as array of coins - blank is also a coin
opt2 - make board as array of pointers to the struct coin
		performing a move is just swapping the pointer
		Might be dangerous, 
*/

enum color COLOR_SELF = COLOR_WHITE;
enum color COLOR_OPP = COLOR_BLACK;

void swapPlayers() {
	int temp;
	temp = COLOR_SELF;
	COLOR_SELF = COLOR_OPP;
	COLOR_OPP = temp;
}

calc (struct  moveset *ms) {
	int i,j;
	struct coin *cn;
	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			cn = &board[i][j];
			debug("Calculating move for %s(%d,%d)\n", cn->name, cn->x, cn->y);
			if(cn->color != COLOR_SELF)
				continue;
			(*cn->calc_moves)(cn, ms);
		}
	}
}

void iterate(struct moveset *ms) {
	int i;
	struct move *mv;
	struct coin *srccn;
	struct coin *dstcn;
	for(i=0; i<ms->validCount; i++) {
		mv = &ms->possible[i];
		printf("Performing %d move %d,%d -> %d->%d\n", i, mv->sx, mv->sy, mv->dx, mv->dy);
		srccn = mv->cn;
		dstcn = &board[mv->dy][mv->dx];
		*dstcn = *srccn;
		*srccn = predefined_coins[6];
		board_print_asciiart();
		*srccn = *dstcn;
		*dstcn = predefined_coins[6];
		getchar();
	}
}

int main() {
	struct moveset ms;
	moveset_init (&ms);
	board_init();
//	board_print();
	board_print_asciiart();
	calc(&ms);
getchar();
	moveset_print(&ms);
	iterate(&ms);
	swapPlayers();
	moveset_init (&ms);
	calc(&ms);
	iterate(&ms);
	board_print_asciiart();
	return 0;
}
