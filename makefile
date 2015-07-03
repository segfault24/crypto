#!/bin/bash

CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -g
LDFLAGS=

all: mymd5.out mysha.out freq.out sep.out caesar.out vigenere.out

%.out: %.o
	$(CC) $(LDFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.out
