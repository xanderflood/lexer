#include "local.h"

#include "string.h"

// static int seek_upwards(JS_EXPR **current);

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
  current = stmt->expressions->data;

  do {
    // semicolon
    if (tok.type == SMC_TOK) {
      // TODO: check that we have a valid statement and then:
      // raise(UNEXPECTED_SEMICOLON)
      return 0;
    }

    // internal-nouns
    else if (tok.type == OPR_TOK) {
      // TODO
      // (1) seek up or down according to operator precedence
      // (2) postfix_push this node in
    }

    // leaf-nouns
    else if (tok.type == NUM_TOK) {
      JS_EXPR *noun = init_expression();
      noun->type = LIT_NUM_EXPR;
      noun->data = strdup(tok.s);
      // TODO: interpret the numeric contents

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    } else if (tok.type == LIT_TOK) {
      JS_EXPR *noun = init_expression();
      noun->type = LIT_STR_EXPR;
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    } else if (tok.type == SYM_TOK) {
      JS_EXPR *noun = init_expression();
      noun->type = SYMBOL_EXPR;
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    } else if (tok.type == PNC_TOK && tok.s[0] == '(') {
      //
    } else if (tok.type == PNC_TOK && tok.s[0] == '[') {
      //
    }

    // keywords should not appear while parsing a statement
    // TODO: except `function`
    else if (tok.type == KEY_TOK)
      return 3; // raise (UNEXPECTED_KEYWORK)

    // otherwise signal characters like ',', ')' ']', ':'
    else {
      update_state(current, &tok);
    }
  } while (!next_token(&ts, 1));

  return 1; // raise(EXPECTED_SEMICOLON)
}

// int read_block(JS_EXPR *block) {
// }

// static int seek_upwards(JS_EXPR **current) {

// }
