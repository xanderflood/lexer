#include <stdio.h>
#include <malloc.h>

typedef char bool;
const bool true = 1;
const bool false = 0;

int main(int n, char **args) {

  // For now, we don't support tokens over 15 chars
  char tok[16];
  bool end_of_token = 0, singleton_token = 0;

  char c, *cur, *base; 
  base = &(tok[0]);

  tok[0] = '\0';
  cur = base;

  while ((c = getchar()) != EOF) {
    
    // surefire end of token
    if (c == ' ' || c == '\n' || c == '\t' || c == '\0') 
      end_of_token = 1;
    // singleton tokens
    else if (c == '{' || c == '}' || c == '(' || c == ')' || c == '-' ||
      c == '+' || c == '*' || c == '^' || c == '/' || c == '!' || c == '%') {
      singleton_token = c;
      end_of_token = 1;
    // regular character
    } else
      *(cur++) = c;

    // print the token
    if (end_of_token) {
      end_of_token = 0;
      if (cur != base) {
        *cur = '\0';
        puts(tok);
        cur = base;
      }
    }

    // print the singleton token
    if (singleton_token) {
      putchar(singleton_token);
      putchar('\n');

      singleton_token = 0;
    }
  }
}
