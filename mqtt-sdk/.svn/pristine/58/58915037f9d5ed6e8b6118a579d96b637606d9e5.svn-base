NATIVE_CC ?= gcc

_all: all

all:
	 $(NATIVE_CC)  \
   	-I/usr/include/libxml2 \
	-L/usr/lib/x86_64-linux-gnu/ \
   	-o tunable tunable_tool.c -lxml2 

clean:
	rm -f tunable

clobber: clean
