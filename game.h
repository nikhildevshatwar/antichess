#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>

//#define DEBUG

#define MOVE_FLAG_RESPAWN	1<<0
#define MOVE_FLAG_KILLED	1<<1

#ifdef DEBUG
	#define debug printf
#else
	#define debug(...)
#endif

#define pos(coin, dir, count) (coin->x + fixed_props[coin->type].x_inc[dir] * count * multiplier(coin)), (coin->y + fixed_props[coin->type].y_inc[dir] * count * multiplier(coin))
#define get_ypos(coin) coin->color == COLOR_WHITE ? coin->y : 7 - coin->y
#define to_coin_data(coinptr) ((struct coin_data *) &(coinptr)->type)

enum direction;
enum color;
enum coin_type;
struct coin;
struct move;
struct moveset;
struct coin_data;
struct coin;
struct coin_properties;

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

struct move {
	struct coin *cn;
	short sx, sy;		//start
	short dx, dy;		//end
	short flags;
	enum coin_type new_coin;	//used only for respawned moves
};

struct moveset {
	int size;
	struct move *possible;
	int validCount;
};

struct coin_data {
	enum coin_type type;
	short color;
};

struct coin {
	short x,y;
	enum coin_type type;
	short color;
};

struct coin_properties {
	char *name;						// name of the coin - can be Blank
	int (*calc_moves)(struct coin *, struct moveset *);	// return no of moves added, and update moveset
	int (*isValid)(struct coin *, struct move *);		// return wheather a move is correct or not
	enum direction allowed[MAX_DIR];			// where all it can move
	short num_dir;
	short x_inc[MAX_DIR];				// table of increments for a specific coin when a move is performed
	short y_inc[MAX_DIR];				// no casteling, so only one value per direction per coin
	char ascii_art[5][6];
};

/******************************** Functions **********************************/
void moveset_init(struct moveset *ms);
void moveset_expand(struct moveset *ms);
void moveset_addMoves(struct moveset *ms, struct coin *cn, short dx, short dy, short flags);
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
extern struct coin_properties fixed_props[];
extern enum color COLOR_OPP;
extern enum color COLOR_SELF;
extern char *dirname[];;
#endif
