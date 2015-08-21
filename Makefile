CXX = g++
CXXFLAGS = -c -Wall -std=c++11
OBJ = main.o ledticker.o

all: ledticker

ledticker: ${OBJ}
	${CXX} -o ledticker ${OBJ}

main.o: main.cc
	${CXX} ${CXXFLAGS} main.cc

ledticker.o: ledticker.cc
	${CXX} ${CXXFLAGS} ledticker.cc

clean:
	rm -rf *o ledticker
