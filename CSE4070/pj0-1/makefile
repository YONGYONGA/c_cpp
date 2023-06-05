testlib:bitmap.o debug.o hash.o hex_dump.o list.o main.o
	gcc -o testlib bitmap.o debug.o hash.o hex_dump.o list.o main.o
main.o: main.c bitmap.h hash.h list.h
	gcc -c -o main.o main.c
bitmap.o: bitmap.c bitmap.h limits.h round.h
	gcc -c -o bitmap.o bitmap.c
debug.o: debug.c debug.h
	gcc -c -o debug.o debug.c
hash.o: hash.c hash.h
	gcc -c -o hash.o hash.c
hex_dump.o:hex_dump.c hex_dump.h round.h
	gcc -c -o hex_dump.o hex_dump.c
list.o: list.h list.c
	gcc -c -o list.o list.c
clean:
	rm *.o testlib

