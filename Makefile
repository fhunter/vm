CFLAGS=-g -Wall -O3
LDFLAGS=-g

all: main j1disasm

main: main.o bitmaps.o state.o memory.o

j1disasm:
	make -C j1disasm

clean:
	rm -f main *.o *.P
	make -C j1disasm clean

doc:
	doxygen Doxyfile

%.o : %.c
	$(CC) -MD -c $(CFLAGS) -o $@ $<
	cp $*.d $*.P
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P
	rm -f $*.d

-include *.P
