min: main.c
	cp main. min.c && sed -rb 's/ {6}//g' main.c | sed -rb 's/\/\/.*$//g' | tr -d '\n' | sed -rb 's/\/\*.*\*\///g' | sed -rb 's/(#include.*>)/\1\n/g'

build: main.c
	gcc -Wall main.c -o main.o

run: main.c
	./main.o $(SQUARE) $(GENERATIONS)

debug: main.c
	gcc -Wall main.c -o main.o && ./main.o 50 50000

clean:
	rm *.o
