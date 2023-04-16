CC = gcc
CFLAGS = -Wall -Wextra

all: codegen_simple

clean:
	rm ./generated/*
	rm ./codegen_simple

codegen_simple: codegen_simple.c CodeGenCliArgs.c CodeGenCliArgs.h
	$(CC) $(CFLAGS) codegen_simple.c CodeGenCliArgs.c -O2 -o codegen_simple
