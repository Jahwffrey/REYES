all: main

main: main.o Ri.o JRi.o
	g++ main.o Ri.o JRi.o -o prog

main.o: main.cpp Ri.h JRi.h
	g++ -c main.cpp

Ri.o: Ri.cpp Ri.h JRi.h
	g++ -c Ri.cpp

JRi.o: JRi.cpp JRi.h Ri.h
	g++ -c JRi.cpp

clean:
	rm prog
	rm *.o
