#include "local.h"

#include "string.h"

// seek upwards to the lowest position that can accept a child, or root
// (never ascends past *unfinished* parentheses)
//  ** Used after any node is placed in a terminal + nonexpectant state
static int seek_upwards(JS_EXPR **current);

// seek downwards as much as possible, minding operator precedence
// (never descends past parentheses)
// ** Used when inserting an operation
static void seek_downwards(JS_EXPR **current, JS_EXPR *opr);

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
    // TODO: After a node accepts a signal token, it's first
    //   child is marked invisible. (In order words, we can't
    //   descend past it until it accepts a new child.)

    //
    // NOTE: here, "noun" means inserted as a child, and "verb" means
    // claiming a child from the syntac tree
    //
    // TODO: `current` is always EITHER expectant or non-terminal
    // No type other than a ROOT_EXPR is ever terminal and expectant at once
    // (1) If it's expectant, then try to interpret `tok` as a noun.
    // (2) If it's not expectant, then it's non-terminal, and waiting on signal tokens.
    // (3) HOWEVER if it is non-expectant, non-terminal AND has
    //  one visible child and the signal token is rejected, we
    //  should try to re-interpret it as a verb operating on that child
    //

    // semicolon
    if (tok.type == SMC_TOK) {
      // TODO: check that we have a valid statement and then:
      // raise(UNEXPECTED_SEMICOLON)
      return 0;
    }

    // verbs
    // TODO: always set `current` to point to this new node
    //       then, seek upwards, in case the new node is already finished (as in `a++;`)
    else if (tok.type == OPR_TOK ||
            (tok.type == PNC_TOK && (tok.s[0] == '(' || tok.s[0] == '['))) {

      // TODO: implement `operator_type(&tok)` to convert a token into an operation expression_type
      // JS_EXPR *noun = init_expression(operator_type(&tok));
      JS_EXPR *opr = init_expression(IND_EXPR);

      seek_downwards(&current, opr);

      if (expr_type_after_the_fact[
          expr_type_fmts[opr->type]])
        add_postfix_child_expression(current, opr);
      else
        add_child_expression(current, opr);

      // TODO '(' and '[' can be:
      // (1) a leaf, when it opens a parenthizes expression
      // (2) an operation, when it signals a function call

      // how to tell? well, if `current` is non-expectant (i.e, it is
      // non-terminal like an unclosed parenthetical) then it *must* be
      // a function call, since function call is in the highet precedence
      // class below parentheticals.

      // same here: it can be a member access (which ties with function call
      // for second-highest precedence class), or it can be an array definition.
    }

    // nouns
    else if (tok.type == NUM_TOK) {
      JS_EXPR *noun = init_expression(LIT_NUM_EXPR);
      noun->data = strdup(tok.s);
      // TODO: interpret the numeric contents

      add_child_expression(current, noun);
    } else if (tok.type == LIT_TOK) {
      JS_EXPR *noun = init_expression(LIT_STR_EXPR);
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
    } else if (tok.type == SYM_TOK) {
      JS_EXPR *noun = init_expression(SYMBOL_EXPR);
      noun->data = strdup(tok.s);

      add_child_expression(current, noun);
    }

    // keywords should not appear while parsing a statement
    // TODO: except `function` - make function an operation
    //       instead of a keyword
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

// A larger value of opr_prec_info means that
// it will burrow *deeper* before pulling
static void seek_downwards(JS_EXPR **current, JS_EXPR *opr) {
  JS_EXPR *target = *current;

  assert(opr_prec_info[opr->type] >= 0);

  while (opr_prec_info[opr->type] > opr_prec_info[target->type]) {
    assert(opr_prec_info[target->type] >= 0);

    target = (*current)->children->data;
  }

  *current = target;
}
