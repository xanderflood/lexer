#include "parse.h"

// returns:
//   0 normal
//   1 EOF
int read_statement(JS_STMT *stmt) {
  TOKEN tok;

  while (true) {
    if (get_token(&tok))
      return 1;

    if (tok->type == SMC_TOK)
      // TODO: check that we have a valid statement?
      return 0;

    // OTHERWISE, push and pop EXPRs from the
    // stmt based on the type of the top EXPR
    // and the type of the incoming token
  }
}

// int read_block(JS_EXPR *block) {
// }

