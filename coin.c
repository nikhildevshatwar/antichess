#include "game.h"

int calc_moves_pawn(struct coin *cn, struct moveset *possible) {
printf("Calc pawn %s\n", cn->name);
	if(isOpp(pos(cn, left_str_diag, 1)))
		moveset_addMoves(possible, left_str_diag, 1);
	if(isOpp(pos(cn, right_str_diag, 1)))
		moveset_addMoves(possible, right_str_diag, 1);
	if(isBlank(pos(cn, str, 1)))
		moveset_addMoves(possible, right_str_diag, 1);
}

int calc_moves_multi(struct coin *cn, struct moveset *possible) {
printf("Calc multi %s\n", cn->name);
	int i, count;
	enum direction dir;

	for(i=0; i<cn->num_dir; dir++) {
		dir = cn->allowed[i];
		count = 0;
		while(isBlank(pos(cn, dir, count)))
			count++;
		if(isOpp(pos(cn, dir, count)))
			count++;
		if(count)
			moveset_addMoves(possible, dir, count);
	}
}

int calc_moves_one(struct coin *cn, struct moveset *possible) {
printf("Calc one %s\n", cn->name);
	int i;
	enum direction dir;

	for(i=0; i<cn->num_dir; dir++) {
		dir = cn->allowed[i];
		if( isNotSelf(pos(cn, dir, 1)))
			moveset_addMoves(possible, dir, 1);
	}
}

int king_isValidMove(struct coin *cn, struct move *mv) {

}

int queen_isValidMove(struct coin *cn, struct move *mv) {

}

int bishop_isValidMove(struct coin *cn, struct move *mv) {

}

int knight_isValidMove(struct coin *cn, struct move *mv) {

}

int rook_isValidMove(struct coin *cn, struct move *mv) {

}

int pawn_isValidMove(struct coin *cn, struct move *mv) {

}
