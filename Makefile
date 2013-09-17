all: play-game

play-game: *.c *.h
	gcc *.c -o play-game

parser: grammer.yacc
	yacc -d 

clean:
	rm -rf *.o play-game
