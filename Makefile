CC=g++

all: memory

memory: memory.o

memory.o: memory.cpp
	g++ -c memory.cpp

run:
	./memory

clean:
	rm *o memory
