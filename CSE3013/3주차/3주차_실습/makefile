target=main
objects = main.o rangearray.o array.o

$(target) : $(objects)
	g++ -o $(target) $(objects)

main.o: main.cpp Array.h RangeArray.h
	g++ -c main.cpp
rangearray.o:rangearray.cpp RangeArray.h
	g++ -c rangearray.cpp
array.o:array.cpp Array.h
	g++ -c array.cpp
clean:
	rm $(target) $(objects)
