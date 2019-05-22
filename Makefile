# IFEM Main Makefile
#
include Makefile.conf
include kernel/Makefile.conf
include boot/Makefile.conf

OBJS=		$(KERNEL_OBJS) $(BOOT_OBJS)

IFEM=		ifem-os

KERNEL_BIN=	kernel.bin
KERNEL_LINK=	kernel.link
KERNEL_ADDR=	0x00000600

BOOT_BIN=	bootblock.bin



all:
	@for MODULE in util kernel boot; do \
		(cd $$MODULE && $(MAKE)); \
	done

	@$(ECHO) $(foreach file, $(KERNEL_OBJS), kernel/$(file)) > \
	$(KERNEL_LINK)

	@$(ECHO) Linking bootblock...
	@$(LD) $(LDFLAGS) --oformat binary -Ttext 0x00000000 -e boot -o $(BOOT_BIN) \
	boot/bootblock.o

	@$(ECHO) Linking kernel...
	@$(LD) $(LDFLAGS) --oformat binary -Ttext $(KERNEL_ADDR) -e startup \
	 -o $(KERNEL_BIN) $$(cat $(KERNEL_LINK))

	@$(CAT) $(BOOT_BIN) $(KERNEL_BIN) > $(IFEM).img

	util/imgsize $(IFEM).img $(KERNEL_BIN)

pad512:
	current_size=$$( stat -c '%s' "$(IFEM).img" ); \
	padding=$$(( 512 - $$(( current_size % 512 )))); \
	$(DD) if=/dev/zero bs=1 count="$${padding}" >> $(IFEM).img


clean:
	@for MODULE in util kernel boot; do \
		(cd $$MODULE && $(MAKE) clean); \
	done

	@$(RM) -f *.o *.bin *.img *.link

install:
	@$(DD) if=$(PNX).img of=$(FD_DEV) bs=$(FD_SECT_SIZE)
