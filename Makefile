CC = gcc
CFLAGS = -Wall -Wextra

all: codegen_simple ./generated/Buffer_Char.h ./generated/Array_Char.h codegen

clean:
	rm ./generated/*
	rm ./codegen_simple
	rm ./codegen

codegen_simple: codegen_simple.c Code_Gen_CLI_Args.c Code_Gen_CLI_Args.h
	$(CC) $(CFLAGS) codegen_simple.c Code_Gen_CLI_Args.c -O2 -o codegen_simple

./generated/Buffer_Char.h: codegen_simple
	./codegen_simple -i templates/Buffer_T.h -o generated/Buffer_Char.h -T char

./generated/Array_Char.h: codegen_simple
	./codegen_simple -i templates/Array_T.h -o generated/Array_Char.h -T char

codegen: codegen.c Code_Gen_CLI_Args.c Code_Gen_CLI_Args.h ./generated/Buffer_Char.h ./generated/Array_Char.h
	$(CC) $(CFLAGS) codegen.c Code_Gen_CLI_Args.c -g -o codegen
