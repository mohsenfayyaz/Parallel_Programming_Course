CC=g++

CFLAGS=-c

OPM=-fopenmp

all: main

main: main.o 
	$(CC) $(OPM) main.o -o main 

main.o: question3_1.cpp
	$(CC) $(CFLAGS) $(OPM) question3_1.cpp -o main.o

clean:
	rm -f *o main

