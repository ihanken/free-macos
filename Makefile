CC=g++

all: memory

memory: memory.o

memory.o: memory.cpp
	g++ -c memory.cpp

clean:
	rm *o memory
