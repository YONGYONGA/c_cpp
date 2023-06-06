target = main

objects = main.o countnum.o inputandmake.o printarr.o

$(target) : $(objects)
	gcc  -o $(target) $(objects)

main.o: main.c header.h
	gcc -c main.c
countnum.o:countnum.c header.h
	gcc -c countnum.c
inputmake.o: inputmake.c header.h
	gcc -c inputmake.c
printarr.o: printarr.c header.h
	gcc -c printarr.c

.PHONY : clean
clean :
	rm $(target) $(objects)
