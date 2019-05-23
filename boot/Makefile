# IFEM Bootloader Makefile
#
include ../Makefile.conf

OBJECTS := bootblock.o
ASFLAGS := --32


all: compile bin
.PHONY: all

###
compile: $(OBJECTS)
.PHONY: compile

###
bootblock.o: bootblock.s segconst.s

###
bin: compile
	@$(LD) $(LDFLAGS) -Ttext $(BOOTBLOCK_ADDR) -e boot -o $(BOOTBLOCK_BIN) \
	$(OBJECTS)
.PHONY: bin

###
clean:
	rm -f *.o *.bin
.PHONY: clean
