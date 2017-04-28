

main: main.c
	gcc -g -O0 *.c -fpic -o lexer

clean:
	rm lexer main.o -f
