# IFEM Bootloader Makefile
#
include ../Makefile.conf

OBJECTS := bootblock.o
ASFLAGS := --32


all: $(OBJECTS)

bootblock.o: bootblock.s segconst.s

bin:
	@$(LD) $(LDFLAGS) -Ttext $(BOOTBLOCK_ADDR) -e boot -o $(BOOTBLOCK_BIN) \
	$(OBJECTS)

clean:
	rm -f *.o *.bin
