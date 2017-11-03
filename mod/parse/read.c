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
  expression_type et;

  // always points to the LOWEST unfinished OPR-type node
  JS_EXPR *new, *current;

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
    // claiming a child from the syntax tree
    //
    // TODO: `current` is always EITHER expectant or non-terminal
    // No type other than a ROOT_EXPR is ever terminal and expectant at once
    //
    // TODO: This doesn't properly handle NTERM_CS_EXST, which is only used by ROOT_EXPR
    //       It's is an expectant state, so it'll never reach the else if it is passed a semicolon
    if (EXPECTANT_EXPR_STATE(current)) {
      // If it's expectant, then try to interpret `tok` as a noun.
      et = interpret_operator_token_as_noun(&tok);

      if (et == IND_EXPR) {
        return -1; // raise UNEXPECTED_????
      } else {
        new = init_expression(et);
        new->data = strdup(tok.s);

        add_child_expression(current, new);

        current = new;
      }
    } else {

      // The only overlap between the signal tokens ";,]):" and
      // the verb tokens is the comma. It should only be used as
      // a verb if it is not accepted as a token, so we try
      // update_state *first*.

      if (! update_state(current, &tok)) {
        // TODO: state was updated, do we need to do anything?
      } else {
        et = interpret_operator_token_as_verb(&tok);
        // seek_downwards
        // add_postfix_child_expression
      }
      // TODO: need to implement child visibility in expression state

      // (2) If it's not expectant, then it's non-terminal, and waiting on signal tokens.
      // (3) HOWEVER if it is non-expectant, non-terminal AND has
      //  one visible child and the signal token is rejected, we
      //  should try to re-interpret it as a verb operating on that child
    }

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

  while (opr_prec_info[opr->type] > opr_prec_info[target->type] && VIS_CHILD_EXPR_STATE(target)) {
    assert(opr_prec_info[target->type] >= 0);

    target = target->children->data;
  }

  *current = target;
}
