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
    if (cur->type == LIT_TOK) {
      ctype = IND_TOK;
      // TODO: call into another function that reads in
      //       the whole string and stores it in `cur`
    } else {
      ctype = diagnoken(c);

      /* Decide whether to coninue */

      // start an empty new token if
      // (-) a whitespace character comes in
      if (ctype == WTS_TOK) {
        next->type = IND_TOK;
        end_of_token = true;
      }

      // put `c` onto the start of a new token if
      // (1) the last character was punctuation (PNC tokens are always 1 char)
      // (2) a semicolon or quote comes in
      else if (cur->type == PNC_TOK || ctype == SMC_TOK || ctype == SMC_TOK) {
        next->type = ctype;
        end_of_token = true;
      }

      // OPR_TOK are only continued by OPR_TOK chars
      // TODO: but not always. properly implement all multi-char operations
      //        is a +++ b valid? is it the same as a ++ +b or a + ++b?
      //        what about a ++++ b vs a ++(++b)
      else if (cur->type == OPR_TOK && ctype != OPR_TOK) {
        next->type = ctype;
        end_of_token = true;
      }

      // numbers and symbols are also terminated by operations and punctuation
      else if ((cur->type == NUM_TOK || cur->type == SYM_TOK)
            && (ctype     == PNC_TOK || ctype     == OPR_TOK)) {
        next->type = ctype;
        end_of_token = true;
      }

      // TODO: check for comments // and /*
    }

    // Add new character to the appropriate token
    if (ctype != WTS_TOK) {
      if (end_of_token)
        ADD_CHAR(*next, c);
      else
        ADD_CHAR(*cur, c);
    }

    if (end_of_token) {
      if (cur->type == SYM_TOK)
        reexamine_symbol(cur);

      // TODO: validate format if OPR_TOK or NUM_TOK

      if (!EMPTY(*cur))
        put_token(cur, text_mode);

      CYCLE;
      INIT_TOKEN(*next);
    } else {
      if (cur->type == IND_TOK)
        cur->type = ctype;
    }

    end_of_token = false;
  }

  return 0;
}
