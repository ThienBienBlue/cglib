CC = gcc
CFLAGS = -Wall -Wextra

all: primitives codegen

clean:
	rm ./primitives/*
	rm ./bootstrap/codegen_simple
	rm ./codegen

primitives:
	mkdir primitives

codegen: codegen.h codegen.c parsing.h parsing.c ./primitives/Buffer_Char.h ./primitives/Buffer_Char.c ./primitives/Array_Char.h ./primitives/Array_Char.c ./primitives/Buffer_Parametric_Binding.h ./primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) codegen.c parsing.c ./primitives/Buffer_Char.c ./primitives/Array_Char.c ./primitives/Buffer_Parametric_Binding.c -O2 -o codegen
