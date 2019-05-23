# IFEM util Makefile
#
include ../Makefile.conf

BINS := imgsize

CFLAGS := -m32 -Wall

all: $(BINS)
.PHONY: all

###
imgsize:
	@$(CC) $(CFLAGS) -o imgsize imgsize.c
.PHONY: imgsize

###
clean:
	rm -f *.o $(BINS)
.PHONY: clean
