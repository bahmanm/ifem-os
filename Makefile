# IFEM Main Makefile
#
include Makefile.conf

all:
	@for MODULE in util foolib kernel boot; do \
		(cd $$MODULE && $(MAKE)); \
	done

	@for MODULE in boot kernel; do \
		(cd $$MODULE && $(MAKE) bin); \
	done

	cat boot/$(BOOTBLOCK_BIN) kernel/$(KERNEL_BIN) > $(IFEM_BIN)

	util/imgsize $(IFEM_BIN) kernel/$(KERNEL_BIN)

pad512:
	current_size=$$( stat -c '%s' "$(IFEM_BIN)" ); \
	padding=$$(( 512 - $$(( current_size % 512 )))); \
	dd if=/dev/zero bs=1 count="$${padding}" >> $(IFEM_BIN)


clean:
	@for MODULE in util foolib kernel boot; do \
		(cd $$MODULE && $(MAKE) clean); \
	done

	@$(RM) -f *.o *.bin *.img *.link

install:
	dd if=$(IFEM_BIN) of=$(FD_DEV) bs=$(FD_SECT_SIZE)
