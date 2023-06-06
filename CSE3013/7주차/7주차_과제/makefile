all	: tetris.o
	gcc tetris.c -lncurses
tetris.o:tetris.h redblack.h tetris.c
	gcc -c tetris.c
clean:
	rm a.out *.o
