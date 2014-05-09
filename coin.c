#include "game.h"

struct coin_properties fixed_props[] = {
	[COIN_KING] = {
		.name = "King",
		.calc_moves = calc_moves_one,
		.isValid = king_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-1, -1, 0, 1, 1, 1, 0, -1},
		.y_inc = {0, 1, 1, 1, 0, -1, -1, -1},
		.num_dir = 8,
		.ascii_art = {
			' ','_','+','_',' ','\0',
			' ','\\',' ','/',' ','\0',
			' ','(',' ',')',' ','\0',
			' ','/',' ','\\',' ','\0',
			'=','=','=','=','=','\0',
		}
	},
	[COIN_QUEEN] = {
		.name = "Queen",
		.calc_moves = calc_moves_multi,
		.isValid = queen_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-1, -1, 0, 1, 1, 1, 0, -1},
		.y_inc = {0, 1, 1, 1, 0, -1, -1, -1},
		.num_dir = 8,
		.ascii_art = {
			' ','x','x','x',' ','\0',
			' ','\\',' ','/',' ','\0',
			' ','(',' ',')',' ','\0',
			' ','/',' ','\\',' ','\0',
			'=','=','=','=','=','\0',
		}
	},
	[COIN_BISHOP] = {
		.name = "Bishop",
		.calc_moves = calc_moves_multi,
		.isValid = bishop_isValidMove,
		.allowed = { left_str_diag, right_str_diag, right_back_diag, left_back_diag },
		.x_inc = {-1, 1, 1, -1},
		.y_inc = {1, 1, -1, -1},
		.num_dir = 4,
		.ascii_art = {
			' ',' ',' ',' ',' ','\0',
			' ',' ','O',' ',' ','\0',
			' ','(','/',')',' ','\0',
			' ','/',' ','\\',' ','\0',
			'_','=','=','=','_','\0',
		}
	},
	[COIN_KNIGHT] = {
		.name = "Knight",
		.calc_moves = calc_moves_one,
		.isValid = knight_isValidMove,
		.allowed = { left, left_str_diag, str, right_str_diag, right, right_back_diag, back, left_back_diag },
		.x_inc = {-2, -2, -1, 1, 2, 2, 1, -1},
		.y_inc = {-1, 1, 2, 2, 1, -1, -2, -2},
		.num_dir = 8,
		.ascii_art = {
			' ',' ',' ',' ',' ','\0',
			' ',',','^','.',' ','\0',
			'(',' ',' ','\'','\\','\0',
			'|',' ',' ','\\',' ','\0',
			'=','=','=','=','_','\0',
		}
	},
	[COIN_ROOK] = {
		.name = "Rook",
		.calc_moves = calc_moves_multi,
		.isValid = rook_isValidMove,
		.allowed = { left, str, right, back },
		.x_inc = {-1, 0, 1, 0},
		.y_inc = {0, 1, 0, -1},
		.num_dir = 4,
		.ascii_art = {
			' ',' ',' ',' ',' ','\0',
			' ','u','u','u',' ','\0',
			' ','|',' ','|',' ','\0',
			' ','/',' ','\\',' ','\0',
			'_','=','=','=','_','\0',
		}
	},
	[COIN_PAWN] = {
		.name = "Pawn",
		.calc_moves = calc_moves_pawn,
		.isValid = pawn_isValidMove,
		.allowed = { left_str_diag, str, right_str_diag },
		.x_inc = {-1, 0, 1 },
		.y_inc = {1, 1, 1 },
		.num_dir = 3,
		.ascii_art = {
			' ',' ',' ',' ',' ','\0',
			' ',' ','_',' ',' ','\0',
			' ','(',' ',')',' ','\0',
			' ','/',' ','\\',' ','\0',
			'_','=','=','=','_','\0',
		}
	},
	[COIN_BLANK] = {
		.name = "Blank",
		.calc_moves = NULL,
		.isValid = NULL,
		.allowed = { },
		.x_inc = { },
		.y_inc = { },
		.num_dir = 0,
		.ascii_art = {
			"     ",
			"     ",
			"     ",
			"     ",
			"_____",
		}
	},
};

int calc_moves_pawn(struct state *st, struct coin *cn, struct moveset *possible) {
	int flags = 0;
	if(get_ypos(cn) == 6)
		flags |= MOVE_FLAG_RESPAWN;
	if(isOpp(st, pos(cn, 0, 1)))			//left_str_diag
		moveset_addMoves(possible, cn, pos(cn, 0, 1), flags | MOVE_FLAG_KILLED);
	if(isBlank(st, pos(cn, 1, 1))) {			//str
		moveset_addMoves(possible, cn, pos(cn, 1, 1), flags);
		if(get_ypos(cn) == 1 && isBlank(st, pos(cn, 1, 2)))		//handle 2moves if at start
			moveset_addMoves(possible, cn, pos(cn, 1, 2), flags);
	}
	if(isOpp(st, pos(cn, 2, 1)))			//right_str_diag
		moveset_addMoves(possible, cn, pos(cn, 2, 1), flags | MOVE_FLAG_KILLED);
}

int calc_moves_multi(struct state *st, struct coin *cn, struct moveset *possible) {
	int i, count;
	enum direction dir;
	for(i=0; i<fixed_props[cn->type].num_dir; i++) {
		dir = fixed_props[cn->type].allowed[i];
		debug("\tmulti>\tdir = %s\n", dirname[dir]);
		count = 1;
		while(isBlank(st, pos(cn, i, count))) {
			moveset_addMoves(possible, cn, pos(cn, i, count), 0);
			count++;
		}
		if(isOpp(st, pos(cn, i, count))) {
			moveset_addMoves(possible, cn, pos(cn, i, count), MOVE_FLAG_KILLED);
			count++;
		}
	}
}

int calc_moves_one(struct state *st, struct coin *cn, struct moveset *possible) {
	int i;
	enum direction dir;
	for(i=0; i<fixed_props[cn->type].num_dir; i++) {
		dir = fixed_props[cn->type].allowed[i];
		debug("\tsingle>\tdir = %s\n", dirname[dir]);
		if( isBlank(st, pos(cn, i, 1)))
			moveset_addMoves(possible, cn, pos(cn, i, 1), 0);
		else if( isOpp(st, pos(cn, i, 1)))
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
