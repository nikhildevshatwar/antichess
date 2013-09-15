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
	ms->possible = malloc(ms->size * sizeof(struct movestep));
}

void moveset_expand(struct moveset *ms) {
	ms->size *= 2;
	ms->possible = realloc(ms->possible, ms->size * sizeof(struct movestep));
}

void moveset_clearAll(struct moveset *ms) {
	ms->validCount = 0;
}

void moveset_addMoves(struct moveset *ms, struct coin *cn, enum direction dir, int count) {
	if(ms->validCount == ms->size) {
		moveset_expand(ms);
	}
	ms->possible[ms->validCount].count = count;
	ms->possible[ms->validCount].dir = dir;
	ms->validCount++;
printf("Adding move no %2d - %s(%d,%d) -> %s -> (%d,%d)\n", ms->validCount, cn->name, cn->x, cn->y, dirname[dir], pos(cn, dir, count));
}

void moveset_print (struct moveset *ms) {
	int i;
	struct movestep *mt;
	for(i=0; i<ms->validCount; i++) 	{
		mt = &ms->possible[i];
		printf("%d %d\n", mt->dir, mt->count);
	}
}
