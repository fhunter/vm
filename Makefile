CFLAGS=-g -Wall -O3
LDFLAGS=-g

all: main

main: main.o bitmaps.o state.o

clean:
	rm -f main *.o *.P

doc:
	doxygen Doxyfile

%.o : %.c
	@g++ -MD -c -o $@ $<
	@cp $*.d $*.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; rm -f $*.d

-include *.P
