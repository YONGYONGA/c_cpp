main : main.o str.o
	g++ -o main main.o str.o
main.o:main.cpp Str.h
	g++ -c main.cpp
str.o:str.cpp Str.h
	g++ -c str.cpp
clean:
	rm main main.o str.o
