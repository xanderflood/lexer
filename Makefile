FLAGS=-ggdb3 -O0 -fPIC -Wall -I include/

all: lex-main parse-main

lex-main: lex mains/lex.c
	gcc $(FLAGS) mains/lex.c mod/lex.so -o bin/lex

parse-main: lex parse mains/parse.c
	gcc $(FLAGS) mains/parse.c mod/lex.so mod/parse.so -o bin/parse

lex: mod/lex/*.c
	gcc $(FLAGS) -shared mod/lex/*.c -o mod/lex.so

parse: mod/parse/*.c
	gcc $(FLAGS) -shared mod/parse/*.c -o mod/parse.so

clean:
	find bin/ ! -name  '.gitkeep' -type f -delete
	rm -rf mod/*.so mod/*/*.o
