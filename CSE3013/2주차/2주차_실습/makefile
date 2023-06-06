CC=gcc
tar=animal_exe
object=dog.o blackcow.o turtle.o main.o
$(tar): $(object)
	$(CC) -o $(tar) $(object)


.PHONY : clean
clean:
	rm $(tar) $(object)
