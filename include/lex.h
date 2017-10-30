#ifndef LEX_H
#define LEX_H

#include "shared.h"

typedef enum {
  // these should never be emitted
  IND_TOK = 0, // indeterminate (not yet known)
  WTS_TOK = 1, // whitespace (not a real token)
  CMT_TOK = 2, // comment token
  INV_TOK = 3, // invalid token (immediate failure)

  // actual values
  NUM_TOK = 4, // number literal
  LIT_TOK = 5, // string literal

  // TODO: should OPR_TOK, PNC_TOK, and SMC_TOK all be merged?

  PNC_TOK = 6, // punctuation (grouping)
  SMC_TOK = 7, // semicolon

  OPR_TOK = 8, // operation (some OPRs look like keywords)
  SYM_TOK = 9, // symbol (variable name )
  KEY_TOK = 10 // control keyword (does not include expr keywords that act as operations)
} token_type;

#define NUM_TOK_TYPES 11
extern const char *token_type_names[NUM_TOK_TYPES];

#define MAX_TOK_SIZE 100
typedef struct {
  char s[MAX_TOK_SIZE];
  char *end;
  token_type type;
} TOKEN;

#define EMPTY(tok)            (&((tok).s[0]) == (tok).end)
#define INIT_TOKEN(tok)  do { (tok).s[0] = '\0'; (tok).end = &((tok).s[0]); (tok).type = IND_TOK; } while (0)
#define ADD_CHAR(tok, c) do { *((tok).end++) = c; *((tok).end) = '\0';                            } while (0)
#define CYCLE            do { tmp = cur; cur = next; next = tmp; INIT_TOKEN(*next);               } while (0)

token_type diagnoken(char c);
void reexamine_symbol(TOKEN *cur);

// writing tokens
void put_token(TOKEN *cur, bool text_mode);

// reading tokens
struct TOKEN_STREAM {
  struct TOKEN_STREAM *next;
  TOKEN data;
};
typedef struct TOKEN_STREAM TOKEN_STREAM;

int init_token_stream(TOKEN_STREAM **ts);
int next_token(TOKEN_STREAM **ts, char destroy);
int peek_token(TOKEN_STREAM *ts, uint k, TOKEN **to);
int seek_token(TOKEN_STREAM *ts, uint k, TOKEN_STREAM **to);
#endif
