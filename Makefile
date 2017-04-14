all: main

main: main.o Ri.o
	g++ main.o Ri.o -o prog

main.o: main.cpp Ri.h
	g++ -c main.cpp

Ri.o: Ri.cpp Ri.h
	g++ -c Ri.cpp

clean:
	rm prog
	rm *.o
