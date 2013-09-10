CFLAGS=-g -Wall -O3
LDFLAGS=-g

all: main

main: main.o bitmaps.o

main.o: main.c ivm.h bitmaps.h

bitmaps.o: bitmaps.c bitmaps.h

clean:
	rm -f main *.o

doc:
	doxygen Doxyfile
