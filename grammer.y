%{
#include <stdio.h>

#define FLAG_CASTLING	1<<0
#define FLAG_KILLING	2<<0
#define FLAG_PROMOTION	3<<0
#define FLAG_THREAT	4<<0

int flags;
char coin, promoted_coin, source_file, dest_file, threat_type;
int source_rank, dest_rank;
int yydebug = 1;

%}

%token SPACE PIECE COIN_FILE COIN_RANK
%start move
%left '+' '#'
%left '='
%left 'x'
%%

move
	: normal_move SPACE
{printf("final\n");}
	| killing_move SPACE		{ flags |= FLAG_KILLING; }
	| casteling SPACE		{ flags |= FLAG_CASTLING; }
	;

casteling
	: exchange
	| exchange threat
	;

exchange
	: 'O' '-' 'O'
	| 'O' '-' 'O' '-' 'O'
	;

killing_move
	: killer 'x' simple_move	{ coin = $1; }
	;

killer
	: COIN_FILE
		{ $$ = 'P'; source_file = $1; }
	| PIECE
		{ $$ = $1; }
	| PIECE COIN_FILE
		{ $$ = $1; source_file = $2; }
	| PIECE COIN_FILE COIN_RANK
		{ $$ = $1; source_file = $2; source_rank = $3; }
	;

normal_move
	: simple_move			{ coin = 'P'; }
	| PIECE simple_move		{ coin = $1; }
	;

simple_move
	: destination
{printf("no prob\n");}
	| destination threat
{printf("threated\n");}
	;

threat
	: '+'			{ threat_type = $1; flags |= FLAG_THREAT; }
	| '#'			{ threat_type = $1; flags |= FLAG_THREAT; }
	;

destination
	: COIN_FILE COIN_RANK
{printf("dest\n");}
		{ dest_file = $1; dest_rank = $2; }
	| COIN_FILE COIN_RANK promotion
{printf("dest and promote\n");}
		{ dest_file = $1; dest_rank = $2; flags |= FLAG_PROMOTION; }
	;

promotion
	: '=' PIECE
	;

%%

#include <stdio.h>

extern char yytext[];
extern int column;

inline char *name(char c) {
	switch(c) {
	case 'K':
		return "King";
	case 'Q':
		return "Queen";
	case 'B':
		return "Bishop";
	case 'N':
		return "Knight";
	case 'R':
		return "Rook";
	case 'P':
		return "Pawn";
	default:
		return "Unknown coin";
	}
}

int main()
{
	char *killing;
	char promotion[32] = "";
	char *threat;
	int idx;

	while(1) {
		killing = "";
		promotion[0] = '\0';
		threat = "";
		flags = 0;

source_file = dest_file = coin = promoted_coin = threat_type = 'Z';
source_rank = dest_rank = 0;


		yyparse();
		printf("Parsing done\n");
		if (flags & FLAG_KILLING) {
			killing = " killed and";
		} else {
			killing = "";
		}
		if (flags & FLAG_PROMOTION) {
			idx = sprintf(promotion, " promotiong to %s", name(promoted_coin));
			promotion[idx] = '\0';
		} else {
			promotion[0] = '\0';
		}
		if (flags & FLAG_THREAT) {
			threat = threat_type == '+' ? " with a check!" : " with Checkmate!!!!";
		} else {
			threat = ".";
		}
		printf ("%s at %c%d%s moved to %c%d%s%s\n", name(coin), \
		source_file, source_rank, killing, dest_file, dest_rank, \
		promotion, threat);
break;
	}
}

yyerror(char *s)
{
	fprintf (stderr, "ERROR: %s\n",s);
}

yywrap()
{
	return 1;
}
