LDFLAGS=-g 
CFLAGS=-g -Wall -I../
VPATH=.:..

all: disassembler vmimage

vmimage: vmimage.o state.o bitmaps.o

clean:
	rm -f disassembler vmimage *.o *.*~
