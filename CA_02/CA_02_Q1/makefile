CC=g++

CFLAGS=-c -msse4.2

# INCDIR=-I/opt/intel/ipp/include
INCDIR=-I/opt/intel/compilers_and_libraries_2020.4.304/linux/ipp/include

# LIBDIR=-L/opt/intel/ipp/lib/intel64
LIBDIR=-L/opt/intel/compilers_and_libraries_2020.4.304/linux/ipp/lib/intel64


LIBFILE=-lippcore

all: main

main: main.o util.o
	$(CC) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp


# main: main.o 
# 	$(CC) main.o $(LIBDIR) $(LIBFILE) -o main

# main.o: main.cpp
# 	$(CC) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main