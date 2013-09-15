#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void move_parse(struct move *mv) {
	//TODO Add logic to parse in the SAN notation
	char sx, dx;
	int sy, dy;

	printf("\n>> ");
	scanf("%c%d %c%d", &sx, &sy, &dx, &dy);
	mv->sx = sx - 'a';
	mv->sy = sy - 1;
	mv->dx = dx - 'a';
	mv->dy = dy - 1;
}

void move_describe(struct move *mv) {
	//TODO Add logic to print in the SAN notation
	printf("%c%d -> %c%d\n", mv->sx + 'a', mv->sy, mv->dx + 'a', mv->dy);
}

void moveset_init(struct moveset *ms) {
	ms->size = 100;
	ms->validCount = 0;
	ms->possible = malloc(ms->size * sizeof(struct move));
}

void moveset_expand(struct moveset *ms) {
	ms->size *= 2;
	ms->possible = realloc(ms->possible, ms->size * sizeof(struct move));
}

void moveset_clearAll(struct moveset *ms) {
	ms->validCount = 0;
}

void moveset_addMoves(struct moveset *ms, struct coin *cn, short dx, short dy, short flags) {
	int i;
	struct move *mv;

	if(ms->validCount == ms->size) {
		moveset_expand(ms);
	}
	mv = &ms->possible[ms->validCount];
	mv->cn = cn;
	mv->sx = cn->x;
	mv->sy = cn->y;
	mv->dx = dx;
	mv->dy = dy;
	mv->flags = flags;
	ms->validCount++;
	printf("Adding move no %2d - %s(%d,%d) -> (%d,%d)\n", ms->validCount, cn->name, cn->x, cn->y, dx, dy);
}

void moveset_print (struct moveset *ms) {
	int i;
	struct move *mv;
	for(i=0; i<ms->validCount; i++) 	{
		mv = &ms->possible[i];
		printf("%s(%d,%d) -> (%d %d)\n", mv->cn->name, mv->sx, mv->sy, mv->dx, mv->dy);
	}
}
