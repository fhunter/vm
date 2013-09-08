CFLAGS=-g -Wall
LDFLAGS=-g

all: main

main: main.o

main.o: main.c ivm.h

clean:
	rm -f main *.o