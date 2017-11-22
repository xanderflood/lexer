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
int read_statement(TOKEN_STREAM *ts, JS_STMT *stmt) {
  TOKEN *tok;
  expression_type et;

  // always points to the LOWEST unfinished OPR-type node
  JS_EXPR *new, *current;

  // TODO: if first token is KEY_TOK || PNC_TOK('{'),
  //       consider other statement types
  // otherwise:

  stmt->type = EXPR_STMT;
  current = stmt->expressions->data;

  //
  // TODO: `current` is always EITHER expectant or non-terminal
  // No type other than a ROOT_EXPR is ever terminal and expectant at once
  //
  do {
    read_token(ts, &tok);

    // There are only a handful of tokens used as signals, and they are all
    // single-character: ";,]):". None of them can be nouns, and only the
    // comma is ever a verb.

    bool signalled = false;
    if (EXPECTING_SIGNAL_STATE(current)) {
      if (! update_state(current, tok)) {
        signalled = true;
      }
    }

    if (! signalled) {
      if (EXPECTANT_EXPR_STATE(current)) {
        // If it's expectant, then try to interpret `tok` as a noun.
        et = interpret_token_as_noun(tok);

        if (et == IND_EXPR) {
          return -1; // raise UNEXPECTED_????
        } else {
          new = init_expression(et);
          new->data = strdup(tok->s);

          add_child_expression(current, new);

          current = new;
        }
      } else if (VIS_CHILD_EXPR_STATE(current)) {
        et = interpret_token_as_verb(tok);

        if (et == IND_EXPR) {
          return -1; // raise UNEXPECTED_????
        } else {
          new = init_expression(et);
          new->data = strdup(tok->s);

          seek_downwards(&current, new);

          add_child_expression(current, new);

          current = new;
        }
      } else {
        return -1; // raise UNRECOGNIZED_TOKEN;
                   // or    EXPECTED_SEMICOLON; ??
      }
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
