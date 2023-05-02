CC = gcc
CFLAGS = -Wall -Wextra

all: ./bootstrap/codegen_simple ./generated/Buffer_Char.h ./generated/Array_Char.h ./generated/Buffer_Char.h ./generated/Array_Char.h ./generated/Array_Parametric_Binding.h ./generated/Array_Parametric_Binding.c codegen

clean:
	rm ./generated/*
	rm ./bootstrap/codegen_simple
	rm ./codegen

./bootstrap/codegen_simple: ./bootstrap/codegen_simple.c ./bootstrap/Code_Gen_CLI_Args.c ./bootstrap/Code_Gen_CLI_Args.h
	$(CC) $(CFLAGS) ./bootstrap/codegen_simple.c ./bootstrap/Code_Gen_CLI_Args.c -g -o ./bootstrap/codegen_simple

./generated/Buffer_Char.h: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Buffer_T.h -o generated/Buffer_Char.h -T char

./generated/Buffer_Char.c: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Buffer_T.c -o generated/Buffer_Char.c -T char

./generated/Array_Char.h: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Array_T.h -o generated/Array_Char.h -T char

./generated/Array_Char.c: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Array_T.c -o generated/Array_Char.c -T char

./generated/Array_Parametric_Binding.h: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Array_T.h -o generated/Array_Parametric_Binding.h -T Parametric_Binding -include ../codegen.h

./generated/Array_Parametric_Binding.c: ./bootstrap/codegen_simple
	./bootstrap/codegen_simple -i templates/Array_T.c -o generated/Array_Parametric_Binding.c -T Parametric_Binding -include ../codegen.h

codegen: codegen.h codegen.c parsing.h parsing.c ./generated/Buffer_Char.h ./generated/Array_Char.h ./generated/Buffer_Char.c ./generated/Array_Char.c
	$(CC) $(CFLAGS) codegen.c parsing.c ./generated/Buffer_Char.c ./generated/Array_Char.c -O2 -o codegen
