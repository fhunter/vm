LIBPATH?=..
VPATH=.:peripherials

CFLAGS=-g -Wall -O0 -DEMU=1 -Werror
CFLAGS_ARM=-fno-common -mcpu=cortex-m3 -mthumb
LDFLAGS=-g
LDFLAGS_ARM =-nostartfiles -T../libs/libopenstm32/libopenstm32.ld -nostdlib
LDFLAGS_ARM+=-L$(LIBPATH)/libs/libopenstm32/
LDFLAGS_ARM+=-L$(LIBPATH)/libs/libusbserial/
LDFLAGS_ARM+=-L$(LIBPATH)/libs/libconio/
LDFLAGS_ARM+=-L$(LIBPATH)/libs/libstm32usb/
LDFLAGS_ARM+=-L$(LIBPATH)/libs/newlib/
LDFLAGS_ARM+=-L/usr/lib/gcc/arm-linux-gnueabi/4.3.5
LDFLAGS_ARM+=-Wl,--start-group -lopenstm32 -lconio -lstm32usb -lusbserial -lgcc -lc -lm -lg -lnosys -Wl,--end-group

PREFIX?= arm-linux-gnueabi-
CC=gcc

INTERMDIR=.build
BINARY_x86=$(INTERMDIR)_x86/main
BINARY_arm=$(INTERMDIR)_arm/main
OBJS = main.o bitmaps.o state.o memory.o vm.o ivm.o launcher.o buttons.o lcd.o
OBJS_arm = $(patsubst %.o, $(INTERMDIR)_arm/%.o, $(OBJS))
OBJS_x86 = $(patsubst %.o, $(INTERMDIR)_x86/%.o, $(OBJS))

all: main utils

main: $(BINARY_x86)

main_arm: $(BINARY_arm)

$(BINARY_x86): $(OBJS_x86)
	@if [ ! -d $(INTERMDIR)_x86 ]; then make $(INTERMDIR)_x86; fi
	$(CC) $(LDFLAGS) -o $@ $^

$(BINARY_arm): $(OBJS_arm)
	@if [ ! -d $(INTERMDIR)_arm ]; then make $(INTERMDIR)_arm; fi
	$(PREFIX)$(CC) $^ $(LDFLAGS) $(LDFLAGS_ARM) -o $@

.build_arm:
	mkdir -p .build_arm

.build_x86:
	mkdir -p .build_x86

utils:
	make -C utils

clean:
	rm -f *.o *.P
	rm -rf $(INTERMDIR)_x86 $(INTERMDIR)_arm
	make -C j1disasm clean

doc:
	doxygen Doxyfile

$(INTERMDIR)_arm/%.o: %.c
	@if [ ! -d $(INTERMDIR)_arm ]; then make $(INTERMDIR)_arm; fi
	$(PREFIX)$(CC) -MD -c $< $(CFLAGS) $(CFLAGS_ARM) -o $@
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
