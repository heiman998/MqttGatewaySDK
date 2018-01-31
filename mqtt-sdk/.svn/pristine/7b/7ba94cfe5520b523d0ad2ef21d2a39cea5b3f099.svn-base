# Copyright (c) 2011 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
NATIVE_CC ?= gcc

_all: all

all: lz77

lz77:	lz.c		\
	main.c		\
	lz.h
	$(NATIVE_CC) -o $@ lz.c main.c

clean:
	rm -f lz77

clobber: clean
