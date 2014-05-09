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

void swapPlayers(struct state *st) {
	int temp;
	temp = st->COLOR_SELF;
	st->COLOR_SELF = st->COLOR_OPP;
	st->COLOR_OPP = temp;
}

calc (struct state *st, struct  moveset *ms) {
	int i,j;
	struct coin *cn;
	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			cn = &(st->board[i][j]);
			debug("Calculating move for %s(%d,%d)\n", fixed_props[cn->type].name, cn->x, cn->y);
			if(cn->color != st->COLOR_SELF)
				continue;
			(*fixed_props[cn->type].calc_moves)(st, cn, ms);
		}
	}
}

void iterate(struct state *st, struct moveset *ms) {
	int i;
	struct move *mv;
	struct coin_data *srcdata;
	struct coin_data *dstdata;
	struct coin_data temp;
	struct coin_data empty = { .color = COLOR_NONE, .type = COIN_BLANK };
	for(i=0; i<ms->validCount; i++) {
		mv = &ms->possible[i];
		printf("Performing %d move %d,%d -> %d->%d\n", i, mv->sx, mv->sy, mv->dx, mv->dy);
		srcdata = to_coin_data(&(st->board[mv->sy][mv->sx]));
		dstdata = to_coin_data(&(st->board[mv->dy][mv->dx]));
	//Make the move
		temp = *dstdata;
		*dstdata = *srcdata;
		*srcdata = empty;
	//Print the board
		board_print_asciiart(st->board);
	//Restore the move
		*srcdata = *dstdata;
		*dstdata = temp;
getchar();
	}
}

struct moveset *make_random_move(struct state *s, struct moveset *ms) {
	int i = rand() % ms->validCount;

	printf("Selected %d randomly", i);
	move_make(s, &ms->possible[i]);
}

crawl(struct state *s, struct moveset *ms) {
	struct moveset newset;
	struct move *mv;

	mv = make_random_move(s, ms);

	swapPlayers(s);
	moveset_init(&newset);
	calc(s, &newset);

	board_print_asciiart(s->board);
	getchar();
	crawl(s, &newset);
//	revert_move(s, mv);
}

int main_check_all_moves() {
	struct moveset ms;
	struct state s1;

	moveset_init (&ms);
	board_init(s1.board);
	s1.COLOR_SELF = COLOR_WHITE;
	s1.COLOR_OPP = COLOR_BLACK;

	board_print(s1.board);
	board_print_asciiart(s1.board);
	getchar();

	calc(&s1, &ms);
	moveset_print(&ms);

	iterate(&s1, &ms);
	swapPlayers(&s1);

	moveset_init (&ms);
	calc(&s1, &ms);
	iterate(&s1, &ms);

	board_print_asciiart(s1.board);

	return 0;
}

int main() {

#ifdef DEBUG
	dbgfp = fopen("debug.log", "w");
#endif
	struct moveset ms;
	struct state s1;

	moveset_init (&ms);
	board_init(s1.board);
	s1.COLOR_SELF = COLOR_WHITE;
	s1.COLOR_OPP = COLOR_BLACK;

	board_print(s1.board);
	board_print_asciiart(s1.board);
	getchar();

	calc(&s1, &ms);
	moveset_print(&ms);

	crawl(&s1, &ms);

}
