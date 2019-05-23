# IFEM Main Makefile
#
include Makefile.conf

all: pad512

.PHONY: all

###
pad512: bootablebin
	current_size=$$( stat -c '%s' "$(IFEM_BIN)" ); \
	padding=$$(( 512 - $$(( current_size % 512 )))); \
	dd if=/dev/zero bs=1 count="$${padding}" >> $(IFEM_BIN)
.PHONY: pad512

###
bootablebin: bin
	cd util && $(MAKE) imgsize
	util/imgsize $(IFEM_BIN) kernel/$(KERNEL_BIN)
.PHONY: bootablebin

###
bin: compile
	@for MODULE in boot kernel; do \
		(cd $$MODULE && $(MAKE) bin); \
	done
	cat boot/$(BOOTBLOCK_BIN) kernel/$(KERNEL_BIN) > $(IFEM_BIN)
.PHONY: bin

###
compile:
	@for MODULE in kernel boot; do \
		(cd $$MODULE && $(MAKE) compile); \
	done
.PHONY: compile

###
clean:
	@for MODULE in util kernel boot; do \
		(cd $$MODULE && $(MAKE) clean); \
	done
	@$(RM) -f *.o *.bin *.img *.link
.PHONY: clean

###
install: all
	dd if=$(IFEM_BIN) of=$(FD_DEV) bs=$(FD_SECT_SIZE)
.PHONY: install
