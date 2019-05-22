# IFEM Kernel Makefile
#
include ../Makefile.conf

OBJECTS := startup.o main.o asmlib.o varalloc.o logdev.o int.o cpu.o isr.o \
           i8259a.o irqhandl.o intrface.o
CFLAGS := $(CFLAGS) -I ../include


all: $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.S.o:
	$(CC) $(CFLAGS) -c $<

bin:
	@$(LD) $(LDFLAGS) --oformat binary -Ttext $(KERNEL_ADDR) -e startup \
	 -o $(KERNEL_BIN) $(OBJECTS)

clean:
	-rm -f *.o *.bin
