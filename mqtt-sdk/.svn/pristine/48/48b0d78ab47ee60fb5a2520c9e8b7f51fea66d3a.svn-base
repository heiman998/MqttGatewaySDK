NATIVE_CC ?= gcc

_all: all

all:
	$(NATIVE_CC) -g -m32 -Wall \
	-I../include \
    -I$(INTERNALDIR)/include \
	-I$(FW)/include \
	-o mkdsetimg mkdsetimg.c lz.c

clean:
	rm -f mkdsetimg

clobber: clean
