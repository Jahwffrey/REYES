all: main

main: main.o Ri.o JRiPoint.o
	g++ main.o Ri.o JRiPoint.o -o prog

main.o: main.cpp Ri.h JRiPoint.h
	g++ -c main.cpp

Ri.o: Ri.cpp Ri.h
	g++ -c Ri.cpp

JRiPoint.o: JRiPoint.cpp JRiPoint.h
	g++ -c JRiPoint.cpp

clean:
	rm prog
	rm *.o
