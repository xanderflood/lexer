#include "local.h"

#include "string.h"

// seek upwards to the lowest position that can accept a child, or root
// (never ascends past *unfinished* parentheses)
//  ** Used after any node is placed in a terminal + nonexpectant state
static int seek_upwards(JS_EXPR **current);

// seek downwards as much as possible, minding operator precedence
// (never descends past parentheses)
// ** Used when inserting an operation
static int seek_downwards(JS_EXPR **current, JS_EXPR *opr);

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

    // verbs
    // TODO: always set `current` to point to this new node
    else if (tok.type == OPR_TOK) {
      // TODO
      // (1) seek down according to operator precedence
      // (2) postfix_push this node in
    } else if (tok.type == PNC_TOK && tok.s[0] == '(') {
      // TODO '(' can be:
      // (1) a leaf, when it opens a parenthizes expression
      // (2) an operation, when it signals a function call

      // how to tell? well, if `current` is non-expectant (i.e, it is
      // non-terminal like an unclosed parenthetical) then it *must* be
      // a function call, since function call is in the highet precedence
      // class below parentheticals.
    } else if (tok.type == PNC_TOK && tok.s[0] == '[') {
      // same here: it can be a member access (which ties with function call
      // for second-highest precedence class), or it can be an array definition.
    }

    // nouns
    else if (tok.type == NUM_TOK) {
      JS_EXPR *noun = init_expression();
      SET_EXPR_TYPE(noun, LIT_NUM_EXPR);
      noun->data = strdup(tok.s);
      // TODO: interpret the numeric contents

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    } else if (tok.type == LIT_TOK) {
      JS_EXPR *noun = init_expression();
      SET_EXPR_TYPE(noun, LIT_STR_EXPR);
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    } else if (tok.type == SYM_TOK) {
      JS_EXPR *noun = init_expression();
      SET_EXPR_TYPE(noun, SYMBOL_EXPR);
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
      // TODO: seek upwards to the next unfinished node
    }

    // keywords should not appear while parsing a statement
    // TODO: except `function`
    else if (tok.type == KEY_TOK)
      return 3; // raise (UNEXPECTED_KEYWORK)

    // signals: signal characters like ',', ')' ']', ':'
    else
      update_state(current, &tok);

    if (seek_upwards(&current))
      return -1; // raise(INVALID_EXPRESSION);

  } while (!next_token(&ts, 1));

  return 1; // raise(EXPECTED_SEMICOLON)
}

// int read_block(JS_EXPR *block) {
// }

static int seek_upwards(JS_EXPR **current) {
  while (FINISHED_EXPR_STATE(*current) && (*current)->parent) {
    *current = (*current)->parent;
  }

  return (INVALID_EXPR_STATE(*current));
}
