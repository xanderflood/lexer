

main: main.c
	gcc -ggdb3 -O0 *.c -fpic -o lexer

clean:
	rm lexer main.o -f
