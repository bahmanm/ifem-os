# IFEM Bootloader Makefile
#
include ../Makefile.conf
include Makefile.conf

OBJS=		$(BOOT_OBJS)


all: $(OBJS)

bootblock.o: bootblock.s segconst.s

clean:
	@$(RM) -f *.o *.bin
	
