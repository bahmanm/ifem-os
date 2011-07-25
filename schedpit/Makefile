# IFEM schedpit/Makefile
#
include ../Makefile.conf
include Makefile.conf

TARGET= 	schedpit.bin
OBJS=		$(SCHEDPIT_OBJS)

all: $(OBJS)

include Makefile.dep

.c.o:
	@$(GCC) $(KERNEL_FLAGS) -c $<

.S.o:
	@$(GCC) $(KERNEL_FLAGS) -c $<
		
clean:
	@$(RM) -f *.o *.bin
	
dep:
	@$(GCC) -M -I$(INCLUDE) *.c > Makefile.dep
	@$(GCC) -M -I$(INCLUDE) *.S >> Makefile.dep
