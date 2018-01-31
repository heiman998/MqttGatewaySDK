# Makefile for certcs certificate server
CC := gcc
CFLAGS := -std=gnu99 -Wno-pointer-to-int-cast -Wall -g -fno-strict-aliasing
certcs: certcs.o utility.o libip.o
	$(CC) $(CFLAGS) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
