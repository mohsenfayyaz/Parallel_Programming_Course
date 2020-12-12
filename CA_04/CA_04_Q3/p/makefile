CC=g++

CFLAGS=-c

OPM=-fopenmp

all: main

main: main.o 
	$(CC) $(OPM) main.o -o main 

main.o: question3_2.cpp
	$(CC) $(CFLAGS) $(OPM) question3_2.cpp -o main.o

clean:
	rm -f *o main

