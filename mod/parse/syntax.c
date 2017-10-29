#include "parse.h"
#include "lex.h"

typedef enum {
  INV_EXP_FMT,

  LEAF_EXPR_FMT,

  // simple operation formats
  ROOT_EXPR_FMT,
  PAREN_EXPR_FMT,
  PREFIX_UNOP_EXPR_FMT,
  POSTFIX_UNOP_EXPR_FMT,
  INFIX_BINOP_EXPR_FMT,

  // compound and n-ary formats
  FCN_CALL_EXPR_FMT,
  FCN_DEFN_EXPR_FMT,
  ARRAY_EXPR_FMT,
  OBJECT_EXPR_FMT,
  COMMA_EXPR_FMT
} expression_format;

const expression_format expr_type_fmts[num_expr_types] = {
  INV_EXP_FMT,       // IND_EXPR

  ROOT_EXPR_FMT,      // ROOT_EXPR

  // nouns (leaves)
  LEAF_EXPR_FMT,          // LIT_NUM_EXPR
  LEAF_EXPR_FMT,          // LIT_STR_EXPR
  LEAF_EXPR_FMT,          // SYMBOL_EXPR
  LEAF_EXPR_FMT,          // DECL_EXPR

  // operations (internal nodes)
  PAREN_EXPR_FMT,        // PAREN_EXPR
  INFIX_BINOP_EXPR_FMT,  // ASSIGN_EXPR
  INFIX_BINOP_EXPR_FMT,  // ASSIGN_PLUS_EXPR
  INFIX_BINOP_EXPR_FMT,  // ASSIGN_TIMES_EXPR
  FCN_CALL_EXPR_FMT,     // CALL_EXPR
  FCN_DEFN_EXPR_FMT,     // FUNCTION_EXPR
  ARRAY_EXPR_FMT,        // ARRAY_EXPR
  OBJECT_EXPR_FMT,       // OBJECT_EXPR
  INFIX_BINOP_EXPR_FMT,  // PLUS_EXPR
  PREFIX_UNOP_EXPR_FMT,  // POST_INCR_EXPR
  POSTFIX_UNOP_EXPR_FMT, // PRE_INCR_EXPR
  INFIX_BINOP_EXPR_FMT,  // TIMES_EXPR
  INFIX_BINOP_EXPR_FMT,  // EQUALS_EXPR
  INFIX_BINOP_EXPR_FMT,  // LT_EXPR
  INFIX_BINOP_EXPR_FMT,  // GT_EXPR
  INFIX_BINOP_EXPR_FMT,  // LET_EXPR
  INFIX_BINOP_EXPR_FMT,  // GET_EXPR

  COMMA_EXPR_FMT         // COMMA_EXPR
};

// `current` has reverted upward to `parent`, meaning
// then all of `parent`s children have a terminal state
// now. A new token appears, and `parent` must tell us
// how to handle it.

// return 0 if the token was not useful (propogate the token  )
int update_state(JS_EXPR *parent, TOKEN *new) {
  assert (EXPR_TYPE_INTERNAL_NODE(parent));

  // if the new token is NULL, then this function
  // is being called because `parent`s first child
  // has now reached a terminal state
  if (!new) {
    if (INV_EXP_FMT) {
      // vacuous
    } else if (LEAF_EXPR_FMT) {
      // vacuous
    } else if (ROOT_EXPR_FMT) {
      // Once ROOT has one terminal child, it becomes terminal too
      parent->state = -1;
    } else if (PAREN_EXPR_FMT) {
      // do nothing, PARENs don't close until a ')' is seen
    } else if (PREFIX_UNOP_EXPR_FMT) {
      parent->state = -1;
    } else if (POSTFIX_UNOP_EXPR_FMT) {
      assert (0);
      // TODO: postfix operations *start out* terminal, and
      // should never get updated
    } else if (INFIX_BINOP_EXPR_FMT) {
      // if (parent->child_count)
    } else if (FCN_CALL_EXPR_FMT) {
    } else if (FCN_DEFN_EXPR_FMT) {
    } else if (ARRAY_EXPR_FMT) {
    } else if (OBJECT_EXPR_FMT) {
    } else if (COMMA_EXPR_FMT) {

    } else {
      // vacuous
      return -1; //raise(UNRECOGNIZED_EXPR_FMT);
    }
  }

  // otherwise, parent's primary child became terminal
  // in a previous step, and this is being called because
  // of a new token, such as a comma or close paren
  else {
    if (INV_EXP_FMT) {
      // vacuous
    } else if (LEAF_EXPR_FMT) {
      // vacuous
    } else if (ROOT_EXPR_FMT) {

    } else if (PAREN_EXPR_FMT) {
    } else if (PREFIX_UNOP_EXPR_FMT) {
    } else if (POSTFIX_UNOP_EXPR_FMT) {
    } else if (INFIX_BINOP_EXPR_FMT) {
    } else if (FCN_CALL_EXPR_FMT) {
    } else if (FCN_DEFN_EXPR_FMT) {
    } else if (ARRAY_EXPR_FMT) {
    } else if (OBJECT_EXPR_FMT) {
    } else if (COMMA_EXPR_FMT) {

    } else {
      // vacuous
      return -1; //raise(UNRECOGNIZED_EXPR_FMT);
    }
  }

  return 0;
}

// -2 = initially terminal AND expectant
// -1 = initially terminal and NOT expectant (i.e. FINISHED)
//  0 = invalid
//  1 = waiting on signal tokens before expecting children
//  2 = initally expectant
const char expr_type_initial_states_[num_expr_types] = {
  0,  // IND_EXPR

  -2, // ROOT_EXPR

  // nouns (leaves)
  -1, // LIT_NUM_EXPR
  -1, // LIT_STR_EXPR
  -1, // SYMBOL_EXPR
  -1, // DECL_EXPR

  // operations (internal nodes)
  2,  // PAREN_EXPR
  2,  // ASSIGN_EXPR
  2,  // ASSIGN_PLUS_EXPR
  2,  // ASSIGN_TIMES_EXPR
  2,  // CALL_EXPR
  1,  // FUNCTION_EXPR
  2,  // ARRAY_EXPR
  2,  // OBJECT_EXPR
  2,  // PLUS_EXPR
  -1, // POST_INCR_EXPR
  2,  // PRE_INCR_EXPR
  2,  // TIMES_EXPR
  2,  // EQUALS_EXPR
  2,  // LT_EXPR
  2,  // GT_EXPR
  2,  // LET_EXPR
  2,  // GET_EXPR

  2   // COMMA_EXPR
};

// does it start expecting a child or no?
void init_state(JS_EXPR *expr) {
  expr->state = expr_type_initial_states_[expr->type];
}

// TODO: build an operator-precedence type of array
