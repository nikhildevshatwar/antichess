#include "game.h"

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
		.color = COLOR_NONE,
		.calc_moves = calc_moves_none,
		.isValid = NULL,
		.num_dir = 0,
	},
};

int calc_moves_none(struct coin *cn, struct moveset *possible) {

}

int calc_moves_pawn(struct coin *cn, struct moveset *possible) {
	int flags = 0;
	if(get_ypos(cn) == 6)
		flags |= MOVE_FLAG_RESPAWN;
	if(isOpp(pos(cn, 0, 1)))			//left_str_diag
		moveset_addMoves(possible, cn, pos(cn, 0, 1), flags | MOVE_FLAG_KILLED);
	if(isBlank(pos(cn, 1, 1))) {			//str
		moveset_addMoves(possible, cn, pos(cn, 1, 1), flags);
		if(get_ypos(cn) == 1 && isBlank(pos(cn, 1, 2)))		//handle 2moves if at start
			moveset_addMoves(possible, cn, pos(cn, 1, 2), 0);
	}
	if(isOpp(pos(cn, 2, 1)))			//right_str_diag
		moveset_addMoves(possible, cn, pos(cn, 2, 1), flags | MOVE_FLAG_KILLED);
}

int calc_moves_multi(struct coin *cn, struct moveset *possible) {
	int i, count;
	enum direction dir;
	for(i=0; i<cn->num_dir; i++) {
		dir = cn->allowed[i];
		debug("\tmulti>\tdir = %s\n", dirname[cn->allowed[i]]);
		count = 1;
		while(isBlank(pos(cn, i, count))) {
			moveset_addMoves(possible, cn, pos(cn, i, count), 0);
			count++;
		}
		if(isOpp(pos(cn, i, count))) {
			moveset_addMoves(possible, cn, pos(cn, i, count), MOVE_FLAG_KILLED);
			count++;
		}
	}
}

int calc_moves_one(struct coin *cn, struct moveset *possible) {
	int i;
	enum direction dir;
	for(i=0; i<cn->num_dir; i++) {
		dir = cn->allowed[i];
		debug("\tsingle>\tdir = %s\n", dirname[cn->allowed[i]]);
		if( isBlank(pos(cn, i, 1)))
			moveset_addMoves(possible, cn, pos(cn, i, 1), 0);
		else if( isOpp(pos(cn, i, 1)))
			moveset_addMoves(possible, cn, pos(cn, i, 1), MOVE_FLAG_KILLED);
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
