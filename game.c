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

calc (struct  moveset *ms) {
	int i,j;
	struct coin *cn;
	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			cn = &board[i][j];
			(*cn->calc_moves)(cn, ms);
		moveset_print(ms);
		}
	}
}

int main() {
	struct moveset ms;
	moveset_init (&ms);
	board_init();
//	board_print();
	board_print_asciiart();
	calc(&ms);
	return 0;
}
