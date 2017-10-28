#include "lex.h"
#include "string.h"

const char *token_type_names[NUM_TOK_TYPES] = {
  "indeterminate",
  "whitespace",
  "comment",
  "invalid",

  "number",
  "literal",

  "punctuation",
  "semicolon",

  "operation",
  "symbol",
  "keyword"
};

token_type diagnoken(char c) {
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
    || c == '$' || c == '_')
    return SYM_TOK;

  if (c >= '0' && c <= '9')
    return NUM_TOK;

  if (c == '\'' || c == '"' || c == '`')
    return LIT_TOK;

  if ( c == '+' || c == '-'  || c == '=' || c == '<'
    || c == '>' || c == '.'  || c == '*' || c == '/'
    || c == '%' || c == '\\' || c == ':' || c == '|'
    || c == '&' || c == '^'  || c == '~' || c == '?')
    return OPR_TOK;

  if (c == ';')
    return SMC_TOK;

  if ( c == '(' || c == ')' || c == '{' || c == '}'
    || c == '[' || c == ']' || c == ',')
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

// All keywords and some operations will get erroneously
// marked as symbols. When a symbol is completed, we need
// to reexamine it.
typedef struct {
  const char *word;
  const token_type type;
} SPECIAL_WORD;

#define NUM_WORDS 17
const SPECIAL_WORD special_words[NUM_WORDS] = {
  { "function",   KEY_TOK },
  { "return",     KEY_TOK },
  { "if",         KEY_TOK },
  { "else",       KEY_TOK },
  { "for",        KEY_TOK },
  { "while",      KEY_TOK },
  { "do",         KEY_TOK },
  { "switch",     KEY_TOK },
  { "case",       KEY_TOK },
  { "break",      KEY_TOK },

  { "in",         OPR_TOK },
  { "new",        OPR_TOK },
  { "typeof",     OPR_TOK },
  { "void",       OPR_TOK },
  { "delete",     OPR_TOK },
  { "instanceof", OPR_TOK },
  { "yield",      OPR_TOK }
};

void reexamine_symbol(TOKEN *cur) {
  uint i;

  for (i = 0; i < NUM_WORDS; i++)
    if (! strcmp(special_words[i].word, cur->s)) {
      cur->type = special_words[i].type;
      return;
    }
}
