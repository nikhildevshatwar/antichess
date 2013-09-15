#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>

#define MOVE_FLAG_RESPAWN	1<<0
#define MOVE_FLAG_KILLED	1<<1

#define pos(coin, dir, count) (coin->x + coin->x_inc[dir] * count), (coin->y + coin->y_inc[dir] * count)

#define debug printf

enum direction;
enum color;
enum coin_type;
struct coin;
struct move;
struct moveset;

enum direction {
	left,
	left_str_diag,
	str,
	right_str_diag,
	right,
	right_back_diag,
	back,
	left_back_diag,
	MAX_DIR
};

enum color {
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_NONE
};

enum coin_type {
	COIN_KING,
	COIN_QUEEN,
	COIN_BISHOP,
	COIN_KNIGHT,
	COIN_ROOK,
	COIN_PAWN,
	COIN_BLANK
};

struct movestep {
	short count;
	enum direction dir;
};

struct moveset {
	int size;
	struct movestep *possible;
	int validCount;
};

struct move {
	short sx, sy;		//start
	short dx, dy;		//end
	int flags;
	enum coin_type new_coin;	//used only for respawned moves
};

struct coin {
	enum coin_type type;
	short color;
	char *name;						// name of the coin - can be Blank
	int (*calc_moves)(struct coin *, struct moveset *);	// return no of moves added, and update moveset
	int (*isValid)(struct coin *, struct move *);		// return wheather a move is correct or not
	enum direction allowed[MAX_DIR];			// where all it can move
	short num_dir;
	short x_inc[MAX_DIR];				// table of increments for a specific coin when a move is performed
	short y_inc[MAX_DIR];				// no casteling, so only one value per direction per coin
	short x,y;					//x and y constant
};

/******************************** Functions **********************************/
void moveset_init(struct moveset *ms);
void moveset_expand(struct moveset *ms);
void moveset_addMoves(struct moveset *ms, struct coin *cn, enum direction dir, int count);
void moveset_clearAll(struct moveset *ms);
void moveset_print (struct moveset *ms);

void move_describe();		//print move in human (chess expert) readble form
void move_parse();		//accept moves from user and convert to struct move

int isOpp(short x, short y);
int isBlank(short x, short y);
int isNotSelf(short x, short y);

int calc_moves_multi(struct coin *cn, struct moveset *possible);
int calc_moves_one(struct coin *cn, struct moveset *possible);
int calc_moves_pawn(struct coin *cn, struct moveset *possible);
int calc_moves_none(struct coin *cn, struct moveset *possible);

int king_isValidMove(struct coin *cn, struct move *mv);
int queen_isValidMove(struct coin *cn, struct move *mv);
int bishop_isValidMove(struct coin *cn, struct move *mv);
int knight_isValidMove(struct coin *cn, struct move *mv);
int rook_isValidMove(struct coin *cn, struct move *mv);
int pawn_isValidMove(struct coin *cn, struct move *mv);

extern struct coin board[8][8];
extern struct coin predefined_coins[];
extern enum color COLOR_OPP;
extern enum color COLOR_SELF;
extern char *dirname[];;
#endif
