CC = gcc
CFLAGS = -Wall -Wextra

all: codegen

clean:
	rm ./types/*
	rm ./codegen

codegen: codegen.c CodeGenCliArgs.c CodeGenCliArgs.h
	$(CC) $(CFLAGS) codegen.c CodeGenCliArgs.c -O2 -o codegen
