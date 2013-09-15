%{
#include "y.tab.h"
%}

%%

[ \n]		{ return SPACE; }
[a-h]		{ return COIN_FILE; }
[1-8]		{ return COIN_RANK; }
[KQBNR]		{ return PIECE; }

