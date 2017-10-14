#include "lex.h"

const char *token_type_names[NUM_TOK_TYPES] = {
  "indeterminate",
  "number",
  "operation",
  "punctuation",
  "symbol",
  "literal",
  "ERROR! (whitespace)",
  "semicolon",
  "invalid"
};

token_type diagnoken(char c) {
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '$' || c == '_')
    return SYM_TOK;

  if (c >= '0' && c <= '9')
    return NUM_TOK;

  if (c == '\'' || c == '"' || c == '`')
    return LIT_TOK;

  if (c == '+' || c == '-' || c == '=' || c == '<' || c == '>'
    || c == '.' || c == '*' || c == '/' || c == '%' ||  c == '\\'
    || c == ':' || c == '|' || c == '&' || c == '^' || c == '~'
    || c == '?')
    return OPR_TOK;

  if (c == ';')
    return SMC_TOK;

  if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'
    || c == ',')
    return PNC_TOK;

  if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
    return WTS_TOK;

  return INV_TOK;
}

bool is_num_continuing(char c) {
  token_type ctype = diagnoken(c);
  return (ctype == PNC_TOK || ctype == LIT_TOK
    || (c == '.'));
}

void put_token(TOKEN *cur, bool text_mode) {
  if (text_mode)
    printf("%s %s\n", token_type_names[cur->type], cur->s);
  else
    printf("%c%s\n", cur->type, cur->s);
}
