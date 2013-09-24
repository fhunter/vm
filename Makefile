CFLAGS=-g -Wall -Os
LDFLAGS=-g
PREFIX?= arm-elf-
CC=gcc

INTERMDIR=.build
BINARY_x86=$(INTERMDIR)_x86/main
BINARY_arm=$(INTERMDIR)_arm/main
SRCS = main.c bitmaps.c state.c memory.c vm.c ivm.c launcher.c
OBJS_arm = $(patsubst %.c, $(INTERMDIR)_arm/%.o, $(SRCS))
OBJS_x86 = $(patsubst %.c, $(INTERMDIR)_x86/%.o, $(SRCS))

all: $(BINARY_x86) $(BINARY_arm) j1disasm

main: $(BINARY_x86)

main_arm: $(BINARY_arm)

$(BINARY_x86): $(OBJS_x86)
	@if [ ! -d $(INTERMDIR)_x86 ]; then make $(INTERMDIR)_x86; fi
	$(CC) -o $@ $^

$(BINARY_arm): $(OBJS_arm)
	@if [ ! -d $(INTERMDIR)_arm ]; then make $(INTERMDIR)_arm; fi
	$(PREFIX)$(CC) -nostartfiles -o $@ $^

.build_arm:
	mkdir -p .build_arm

.build_x86:
	mkdir -p .build_x86

j1disasm:
	make -C j1disasm

clean:
	rm -f *.o *.P
	rm -rf $(INTERMDIR)_x86 $(INTERMDIR)_arm
	make -C j1disasm clean

doc:
	doxygen Doxyfile

$(INTERMDIR)_arm/%.o: %.c
	@if [ ! -d $(INTERMDIR)_arm ]; then make $(INTERMDIR)_arm; fi
	$(PREFIX)$(CC) -MD -c $< $(CFLAGS) -o $@
	cp $(INTERMDIR)_arm/$*.d $(INTERMDIR)_arm/$*.P
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $(INTERMDIR)_arm/$*.d >> $(INTERMDIR)_arm/$*.P
	rm -f $(INTERMDIR)_arm/$*.d

$(INTERMDIR)_x86/%.o: %.c
	@if [ ! -d $(INTERMDIR)_x86 ]; then make $(INTERMDIR)_x86; fi
	$(CC) -MD -c $< $(CFLAGS) -o $@
	cp $(INTERMDIR)_x86/$*.d $(INTERMDIR)_x86/$*.P
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $(INTERMDIR)_x86/$*.d >> $(INTERMDIR)_x86/$*.P
	rm -f $(INTERMDIR)_x86/$*.d

-include $(INTERMDIR)_arm/*.P
-include $(INTERMDIR)_x86/*.P

.PHONY: j1disasm main main_arm
