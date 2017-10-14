// for `getopt`
#include <unistd.h>

#include "lex.h"

int main(int narg, char **args) {
  TOKEN a, b;
  TOKEN *cur = &a;
  TOKEN *next = &b;
  TOKEN *tmp;

  token_type ctype;

  bool end_of_token = false;
  bool text_mode = false;

  char opt;
  while ((opt = getopt(narg, args, "t")) != -1) {
    switch (opt) {
    case 't': text_mode = true; break;
    default:
      fprintf(stderr, "Usage: %s [-t] [file...]\n", args[0]);
      exit(EXIT_FAILURE);
    }
  }

  INIT_TOKEN(*cur);
  INIT_TOKEN(*next);

  char c;
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

      // and a semicolon
      else if (ctype == SMC_TOK) {
        ADD_CHAR(*next, c);
        next->type = SMC_TOK;
        end_of_token = true;
      }

      // and does a quote
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


      // TODO check for comments
    // } else {
    //   // TODO: string literal parsing needs to find unescaped matching quotes!
    // }

    if (end_of_token) {
      if (!EMPTY(*cur))
        put_token(cur, text_mode);

      SWAP;
      INIT_TOKEN(*next);

      end_of_token = false;
    } else {
      if (ctype != WTS_TOK)
        ADD_CHAR(*cur, c);

      if (cur->type == IND_TOK)
        cur->type = ctype;
    }
  }
}
