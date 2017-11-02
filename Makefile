COMPILE=gcc -ggdb3 -O0 -fPIC -Wall -I include/ $(FLAGS)

all: lex-main parse-main

lex-main: lex mains/lex.c
	$(COMPILE) mains/lex.c mod/lex.so -o bin/lex

parse-main: lex parse mains/parse.c
	$(COMPILE) mains/parse.c mod/lex.so mod/parse.so -o bin/parse

lex: mod/lex/*.c
	$(COMPILE) -shared mod/lex/*.c -o mod/lex.so

parse: mod/parse/*.c
	$(COMPILE) -shared mod/parse/*.c -o mod/parse.so

clean:
	find bin/ ! -name  '.gitkeep' -type f -delete
	rm -rf mod/*.so mod/*/*.o
