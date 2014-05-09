CFLAGS = -g
#CFLAGS += -Wall -pedantic -ansi -std=c99

all: play-game

play-game: *.c *.h
	gcc *.c $(CFLAGS) -o play-game

parser: grammer.yacc
	yacc -d 

clean:
	rm -rf *.o play-game
