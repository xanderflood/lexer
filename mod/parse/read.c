#include "parse.h"
#include "lex.h"

// returns:
//   0 OK
//   1 expected semicolon
int read_statement(JS_STMT *stmt) {
  TOKEN tok;
  TOKEN_STREAM *ts;
  int ret;

  if ((ret = init_token_stream(&ts)) < 0)
    return ret;

  do {
    if (tok.type == SMC_TOK)
      // TODO: check that we have a valid statement and then:
      // raise(UNEXPECTED_SEMICOLON)
      return 0;

  } while (!next_token(&ts, 1));

  return 1; // raise(EXPECTED_SEMICOLON)
}

// int read_block(JS_EXPR *block) {
// }
