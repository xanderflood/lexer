#include <stdio.h>
#include <malloc.h>

typedef enum {
  IND_TOK, // indeterminate (not yet known)
  NUM_TOK, // number literal
  OPR_TOK, // operation
  PNC_TOK, // punctuation (grouping)
  SYM_TOK, // symbol (variable name or keyword)
  LIT_TOK, // string literal
  WTS_TOK, // whitespace (not a real token)
  INV_TOK  // invalid token (immediate failure)
} token_type;

const char *token_type_names[8] = {
  "indeterminate",
  "number",
  "operation",
  "punctuation",
  "symbol",
  "literal",
  "ERROR! (whitespace)",
  "invalid"
};

#define MAX_TOK_SIZE 100
typedef struct {
  char s[MAX_TOK_SIZE];
  char *end;
  token_type type;
} TOKEN;
#define EMPTY(tok) (&((tok).s[0]) == (tok).end)

#define INIT_TOKEN(tok)  (tok).s[0] = '\0'; (tok).end = &((tok).s[0]);
#define ADD_CHAR(tok, c) *((tok).end++) = c; *((tok).end) = '\0';
#define SWAP             tmp = cur; cur = next; next = tmp; INIT_TOKEN(*next);

typedef char bool;
const bool true = 1;
const bool false = 0;

token_type diagnoken(char c);
bool is_num_continuing(char c);

int main(int n, char **args) {

  TOKEN a, b;
  TOKEN *cur = &a;
  TOKEN *next = &b;
  TOKEN *tmp;

  token_type ctype;

  unsigned int scope_depth;
  bool end_of_token;
  bool first_token = true;
  int i;

  INIT_TOKEN(a);
  INIT_TOKEN(b);

  char c;
  cur->type == IND_TOK;
  while ((c = getchar()) != EOF) {

    // Only do regular tokenizing if we're not
    // parsing a string literal.
    // if (cur->type == LIT_TOK) {
      ctype = diagnoken(c);

      // whitespace always ends a token
      if (ctype == WTS_TOK) {
        next->type = IND_TOK;
        end_of_token = true;
      }

      // so does a quote
      else if (ctype == LIT_TOK) {
        ADD_CHAR(*next, c);
        next->type = LIT_TOK;
        end_of_token = true;
      }

      // punctuation always ends immediately
      else if (cur->type == PNC_TOK) {
        ADD_CHAR(*next, c);
        next->type = ctype;
        end_of_token = true;
      }

      // (outside of quote mode) a quote always
      // starts a new token.
      // when parsing an operation, a symbol,
      // number, punctuation or quote chars will
      // start a new token.
      else if (cur->type == LIT_TOK || (cur->type == OPR_TOK &&
         (ctype == SYM_TOK || ctype == NUM_TOK || PNC_TOK || LIT_TOK))) {
        ADD_CHAR(*next, c);
        next->type = ctype;
        end_of_token = true;
      }
      // TODO: can operations also be terminated by certain operations?
      // TODO: for instance, is `a =++b` legal syntax?
      // TODO: maybe operators only continue on '='? but what about ++ then?

      // numbers are terminated by an operation,
      // punctuation, literal or whitespace char
      else if (cur->type == NUM_TOK &&
         (ctype == PNC_TOK || ctype == OPR_TOK || ctype == LIT_TOK)) {
        ADD_CHAR(*next, c);
        next->type = ctype;
        end_of_token = true;
      }

      else if (cur->type == SYM_TOK &&
        (ctype == OPR_TOK || ctype == PNC_TOK || ctype == LIT_TOK)) {
        ADD_CHAR(*next, c);
        next->type = ctype;
        end_of_token = true;
      }

      // TODO if punctuation, adjust scope_depth

      // TODO check for comments
    // } else {
    //   // TODO: string literal parsing needs to find unescaped matching quotes!
    // }

    if (end_of_token) {
      if (!EMPTY(*cur)) {
        puts(token_type_names[cur->type]);
        puts(cur->s);
      }

      SWAP;

      end_of_token = false;
    } else {
      ADD_CHAR(*cur, c);

      if (cur->type == IND_TOK) {
        cur->type = ctype;
        first_token = false;
      }
    }
  }
}

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

  if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'
    || c == ';' || c == ',')
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
