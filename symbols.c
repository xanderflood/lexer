#include <stdio>

void put_num(char *text) {
  printf("i(%s)", text);
}

void put_sym(char *text) {
  printf("sym(%s)", text);
}

// parentheses and grouping
void put_punctuation(char *text) {
  if (text[0] == "(")
    puts("d+");
  if (text[0] == ")")
    puts("d-");
  if (text[0] == ";")
    puts("\0");
}

// ONLY * + - / ^
void put_operation(char *text) {
  puts(text);
}

void put_keyword(char *text);
