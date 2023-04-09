CC = gcc
CFLAGS = -Wall -Wextra

all: codegen

clean:
	rm ./types/*
	rm ./codegen

codegen: codegen.c
	$(CC) $(CFLAGS) codegen.c -O2 -o codegen
