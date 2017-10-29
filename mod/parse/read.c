#include "parse.h"
#include "lex.h"

// returns:
//   0 OK
//   1 expected semicolon
int read_statement(JS_STMT *stmt) {
  int ret;
  TOKEN tok;
  TOKEN_STREAM *ts;

  // always points to the LOWEST unfinished OPR-type node
  JS_EXPR *current;

  if ((ret = init_token_stream(&ts)) < 0)
    return ret;

  // TODO: if first token is KEY_TOK || PNC_TOK('{'),
  // consider other statement types
  // otherwise:

  stmt->type = EXPR_STMT;

  current = init_expression();
  current->type = ROOT_EXPR;

  push_expression(&stmt->expressions, current);

  do {
    // semicolon
    if (tok.type == SMC_TOK) {
      // TODO: check that we have a valid statement and then:
      // raise(UNEXPECTED_SEMICOLON)
      return 0;
    }

    // parentheses
    if (tok.type == PNC_TOK) {
      if (tok.s[0] == '(');
        // TODO: push a PAREN_EXPR

      else if (tok.s[0] == ')');
        // TODO: seek upwards to a PAREN_EXPR and close it

      else
        return 2; // raise(UNEXPECTED_PNC_TOK);
    }

    // operations
    else if (tok.type == OPR_TOK) {
      // TODO
      // (1) seek up or down according to operator precedence
      // (2) postfix_push this node in
    }

    // nouns
    // TODO: each one should
    // (1) build an appropriate leaf node
    // (2) push onto current
    else if (tok.type == NUM_TOK) {
      //
    } else if (tok.type == LIT_TOK) {
      //
    } else if (tok.type == SYM_TOK) {
      //
    }

    // otherwise, errors
    else if (tok.type == KEY_TOK)
      return 3; // raise (UNEXPECTED_KEYWORK)
    else
      return 4; // raise(INVALID_TOKEN_TYPE)
  } while (!next_token(&ts, 1));

  return 1; // raise(EXPECTED_SEMICOLON)
}

// int read_block(JS_EXPR *block) {
// }
