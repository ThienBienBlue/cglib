CC = gcc
CFLAGS = -Wall -Wextra

all: ./bootstrap/codegen_simple ./generated/Buffer_Char.h ./generated/Array_Char.h codegen

clean:
	rm ./generated/*
	rm ./bootstrap/codegen_simple
	rm ./codegen

./bootstrap/codegen_simple: ./bootstrap/codegen_simple.c ./bootstrap/Code_Gen_CLI_Args.c ./bootstrap/Code_Gen_CLI_Args.h
	$(CC) $(CFLAGS) ./bootstrap/codegen_simple.c ./bootstrap/Code_Gen_CLI_Args.c -O2 -o ./bootstrap/codegen_simple

./generated/Buffer_Char.h: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Buffer_T.h -o generated/Buffer_Char.h -T char

./generated/Array_Char.h: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Array_T.h -o generated/Array_Char.h -T char

codegen: codegen.c parsing.h parsing.c ./bootstrap/Code_Gen_CLI_Args.c ./bootstrap/Code_Gen_CLI_Args.h ./generated/Buffer_Char.h ./generated/Array_Char.h
	$(CC) $(CFLAGS) codegen.c parsing.c ./bootstrap/Code_Gen_CLI_Args.c -g -o codegen
