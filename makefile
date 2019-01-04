EXECS=simulated_annealing
CPP=g++
VERSION=c++11

all: knn.o ${EXECS}.o
	g++ -o simulated_annealing knn.o simulated_annealing.o

simulated_annealing.o: simulated_annealing.cpp knn.hpp
	${CPP} -o simulated_annealing.o -c -std=${VERSION} simulated_annealing.cpp -O3

knn.o: knn.cpp
	g++ -std=${VERSION} -o knn.o -c knn.cpp -lm -O3

run:
	./${EXECS}
clean:
	rm -rf *.o

mrproper: clean
	rm -rf ${EXECS}