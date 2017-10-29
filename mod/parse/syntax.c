#include "parse.h"
#include "lex.h"

// each expression_type has an associated
// expression_format which determines its
// internal state machine
typedef enum {
  INV_EXP_FMT,

  LEAF_EXPR_FMT,

  // simple operation formats
  ROOT_EXPR_FMT,

  // noun formats
  VAR_DECL_EXPR_FMT,
  ARRAY_EXPR_FMT,
  OBJECT_EXPR_FMT,
  FCN_DEFN_EXPR_FMT,

  PREFIX_UNOP_EXPR_FMT,
  POSTFIX_UNOP_EXPR_FMT,
  INFIX_BINOP_EXPR_FMT,

  PAREN_EXPR_FMT,
  COMP_ACCESS_EXPR_FMT,
  NEW_ARGS_EXPR_FMT,
  FCN_CALL_EXPR_FMT,

  CONF_TRIOP_EXPR_FMT
} expression_format;

#define num_expression_formats 15

const expression_format expr_type_fmts[num_expr_types] = {
  INV_EXP_FMT,   // IND_EXPR      indeterminate

  ROOT_EXPR_FMT, // ROOT_EXPR     placeholder at the top of a statement's expr tree

  // noun literals (leaves)
  LEAF_EXPR_FMT, // LIT_NUM_EXPR  numeric literal
  LEAF_EXPR_FMT, // LIT_STR_EXPR  string literal
  LEAF_EXPR_FMT, // SYMBOL_EXPR   string data

  // noun expressions
  VAR_DECL_EXPR_FMT, // DECL_EXPR      string data (similar to SYMBOL but declares a )
  ARRAY_EXPR_FMT,    // ARRAY_EXPR     arbitrary list of expressions
  OBJECT_EXPR_FMT,   // OBJECT_EXPR    alternates between symbols and RVALUE types
  FCN_DEFN_EXPR_FMT, // FUNCTION_EXPR  a function definition

  // operations (internal nodes)
  PAREN_EXPR_FMT,       // PAREN_EXPR         (a)
  INFIX_BINOP_EXPR_FMT, // ACCESS_EXPR        a.b
  COMP_ACCESS_EXPR_FMT, // COMP_ACCESS_EXPR   a[b]
  NEW_ARGS_EXPR_FMT,    // NEW_ARGS_EXPR      new a (b, ...)
  FCN_CALL_EXPR_FMT,    // CALL_EXPR          a(...)

  PREFIX_UNOP_EXPR_FMT, // NEW_EXPR           new a

  POSTFIX_UNOP_EXPR_FMT, // POST_INCR_EXPR     a ++
  POSTFIX_UNOP_EXPR_FMT, // POST_DECR_EXPR     a --

  // prefix unary operations
  PREFIX_UNOP_EXPR_FMT, // NOT_EXPR           ! a
  PREFIX_UNOP_EXPR_FMT, // BITNOT_EXPR        ~ a
  PREFIX_UNOP_EXPR_FMT, // POS_EXPR           + a
  PREFIX_UNOP_EXPR_FMT, // NEG_EXPR           - a
  PREFIX_UNOP_EXPR_FMT, // PRE_INCR_EXPR      ++ a
  PREFIX_UNOP_EXPR_FMT, // PRE_DECR_EXPR      -- a
  PREFIX_UNOP_EXPR_FMT, // TYPEOF_EXPR        typdeof a
  PREFIX_UNOP_EXPR_FMT, // VOID_EXPR          void a
  PREFIX_UNOP_EXPR_FMT, // DELETE_EXPR        delete a

  INFIX_BINOP_EXPR_FMT, // EXP_EXPR           a ** b

  INFIX_BINOP_EXPR_FMT, // TIMES_EXPR         a * b
  INFIX_BINOP_EXPR_FMT, // QUOT_EXPR          a / b
  INFIX_BINOP_EXPR_FMT, // MOD_EXPR           a % b

  INFIX_BINOP_EXPR_FMT, // PLUS_EXPR          a + b
  INFIX_BINOP_EXPR_FMT, // MINUS_EXPR         a - b

  INFIX_BINOP_EXPR_FMT, // BSLEFT_EXPR        a << b
  INFIX_BINOP_EXPR_FMT, // BSRIGHT_EXPR       a >> b
  INFIX_BINOP_EXPR_FMT, // UBSRIGHT_EXPR      a >>> b

  INFIX_BINOP_EXPR_FMT, // LT_EXPR            a < b
  INFIX_BINOP_EXPR_FMT, // GT_EXPR            a > b
  INFIX_BINOP_EXPR_FMT, // LET_EXPR           a <= b
  INFIX_BINOP_EXPR_FMT, // GET_EXPR           a >= b
  INFIX_BINOP_EXPR_FMT, // IN_EXPR            a in b
  INFIX_BINOP_EXPR_FMT, // INSTANCEOF_EXPR    a instanceof b

  INFIX_BINOP_EXPR_FMT, // EQ_EXPR            a == b
  INFIX_BINOP_EXPR_FMT, // INEQ_EXPR          a != b
  INFIX_BINOP_EXPR_FMT, // STRICT_EQ_EXPR     a === b
  INFIX_BINOP_EXPR_FMT, // STRICT_INEQ_EXPR   a !== b

  INFIX_BINOP_EXPR_FMT, // BAND_EXPR          a & b

  INFIX_BINOP_EXPR_FMT, // BXOR_EXPR          a ^ b

  INFIX_BINOP_EXPR_FMT, // BOR_EXPR           a | b

  INFIX_BINOP_EXPR_FMT, // LAND_EXPR          a && b

  INFIX_BINOP_EXPR_FMT, // LOR_EXPR           a || b

  CONF_TRIOP_EXPR_FMT,  // COND_EXPR          a ? b : c

  INFIX_BINOP_EXPR_FMT, // ASSIGN_EXPR           a = b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_PLUS_EXPR      a += b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_TIMES_EXPR     a *= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_EXP_EXPR       a **= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_QUOT_EXPR      a /= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_MOD_EXPR       a %= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_BSLEFT_EXPR    a <<= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_BSRIGHT_EXPR   a >>= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_UBSRIGHT_EXPR  a >>>= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_BAND_EXPR      a &= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_BXOR_EXPR      a ^= b
  INFIX_BINOP_EXPR_FMT, // ASSIGN_BOR_EXPR       a |= b

  PREFIX_UNOP_EXPR_FMT, // YIELD_EXPR            yield a
  PREFIX_UNOP_EXPR_FMT, // YIELD_STAR_EXPR       yield *a

  PREFIX_UNOP_EXPR_FMT, // SPREAD_EXPR           ... a

  INFIX_BINOP_EXPR_FMT // COMMA_EXPR            a, b (valuates to b)
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
  }

  // otherwise, parent's primary child became terminal
  // in a previous step, and this is being called because
  // of a new token, such as a comma or close paren
  else {
  }

  return 0;
}

// -2 = initially terminal AND expectant
// -1 = initially terminal and NOT expectant (i.e. FINISHED)
//  0 = invalid
//  1 = waiting on signal tokens before expecting children
//  2 = initally expectant and non-terminal
const char expr_type_initial_states[num_expression_formats] = {
  -1, // LEAF_EXPR_FMT

  -2, // ROOT_EXPR_FMT

  2,  // VAR_DECL_EXPR_FMT
  2,  // ARRAY_EXPR_FMT
  2,  // OBJECT_EXPR_FMT
  2,  // FCN_DEFN_EXPR_FMT

  2,  // PREFIX_UNOP_EXPR_FMT
  -1, // POSTFIX_UNOP_EXPR_FMT
  2,  // INFIX_BINOP_EXPR_FMT

  2,  // PAREN_EXPR_FMT
  2,  // COMP_ACCESS_EXPR_FMT
  2,  // NEW_ARGS_EXPR_FMT
  2,  // FCN_CALL_EXPR_FMT

  2  // CONF_TRIOP_EXPR_FMT
};

// does it start expecting a child or no?
void init_state(JS_EXPR *expr) {
  expr->state = expr_type_initial_states[expr->type];
}

// TODO: build an operator-precedence type of array
