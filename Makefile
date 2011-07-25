# IFEM Main Makefile
#
include Makefile.conf
include kernel/Makefile.conf
include schedpit/Makefile.conf
include boot/Makefile.conf
include schedpit/link.addr

OBJS=		$(KERNEL_OBJS) $(SCHEDPIT_OBJS) $(BOOT_OBJS)

KERNEL_ADDR=	0x00000600
IFEM=		ifem-os
BOOT=		bootblock
SCHEDPIT=	schedpit

KERNEL_LINK=	kernel.link
SCHEDPIT_LINK=	schedpit.link

BOOT_BIN=	$(BOOT).bin
IFEM_BIN=	$(IFEM).bin
SCHEDPIT_BIN=	$(SCHEDPIT).bin


all:
	@for MODULE in util kernel schedpit boot; do \
		(cd $$MODULE && $(MAKE)); \
	done
	
	@$(ECHO) $(foreach file, $(KERNEL_OBJS), kernel/$(file)) > \
	$(KERNEL_LINK)
	@$(ECHO) $(foreach file, $(SCHEDPIT_OBJS), schedpit/$(file)) > \
	$(SCHEDPIT_LINK)
	
	@$(ECHO) Linking bootblock...
	@$(LD) --oformat binary -Ttext 0x00000000 -e boot -o $(BOOT_BIN) \
	boot/$(BOOT).o
	
	@$(ECHO) Linking kernel...
	@$(LD) --oformat binary -Ttext $(KERNEL_ADDR) -e startup \
	 -o $(IFEM_BIN)	$$(cat $(KERNEL_LINK))

	@$(ECHO) Linking schedpit...
	@util/schedpit_addr $(IFEM_BIN) schedpit/link.addr
	@$(LD) --oformat binary -Ttext $(SCHEDPIT_ADDR) -e schedpit_entry \
	-o $(SCHEDPIT_BIN) $$(cat $(SCHEDPIT_LINK))

	@$(CAT) $(BOOT_BIN) $(IFEM_BIN) $(SCHEDPIT_BIN) > $(IFEM).img

	@util/imgsize $(IFEM).img
	
clean:
	@for MODULE in util kernel schedpit boot; do \
		(cd $$MODULE && $(MAKE) clean); \
	done
	
	@$(RM) -f *.o *.bin *.img *.link
	
install:
	@$(DD) if=$(PNX).img of=$(FD_DEV) bs=$(FD_SECT_SIZE)
