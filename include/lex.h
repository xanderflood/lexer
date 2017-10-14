#ifndef LEX_H
#define LEX_H

#include "shared.h"

typedef enum {
  IND_TOK, // indeterminate (not yet known)
  NUM_TOK, // number literal
  OPR_TOK, // operation
  PNC_TOK, // punctuation (grouping)
  SYM_TOK, // symbol (variable name or keyword)
  LIT_TOK, // string literal
  WTS_TOK, // whitespace (not a real token)
  SMC_TOK, // semicolon
  INV_TOK  // invalid token (immediate failure)
} token_type;

#define NUM_TOK_TYPES 9
extern const char *token_type_names[NUM_TOK_TYPES];

#define MAX_TOK_SIZE 100
typedef struct {
  char s[MAX_TOK_SIZE];
  char *end;
  token_type type;
} TOKEN;

#define EMPTY(tok)       (&((tok).s[0]) == (tok).end)
#define INIT_TOKEN(tok)  (tok).s[0] = '\0'; (tok).end = &((tok).s[0]); (tok).type = IND_TOK;
#define ADD_CHAR(tok, c) *((tok).end++) = c; *((tok).end) = '\0';
#define SWAP             tmp = cur; cur = next; next = tmp; INIT_TOKEN(*next);

token_type diagnoken(char c);
bool is_num_continuing(char c);
void put_token(TOKEN *cur, bool text_mode);

#endif
