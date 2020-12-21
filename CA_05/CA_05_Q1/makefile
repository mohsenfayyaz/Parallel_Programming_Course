CC=g++

CFLAGS=-c

OPM=-fopenmp

POSIX=-lpthread

all: main

main: main.o 
	$(CC) $(OPM) main.o -o main 

main.o: main.cpp
	$(CC) $(CFLAGS) $(OPM) $(POSIX) main.cpp -o main.o

clean:
	rm -f *o main

