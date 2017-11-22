#include "local.h"

// Tells whether or not an operator expression will be
// built *after* the first child expression it receives
// TODO: Any node that becomes a child of a node created *after*
//       it is declared *finished* and becomes invisible to the
//       AST building process.
const EXPR_FORMAT_INFO format_info[NUM_EXPR_FORMATS] = {
  // INV_EXPR_FMT
  { .size = 0,  .after_the_fact = -1    },

  // LEAF_EXPR_FMT
  { .size = -1, .after_the_fact = false },

  // ROOT_EXPR_FMT
  { .size = 1,  .after_the_fact = false },

  // VAR_DECL_EXPR_FMT
  { .size = 1,  .after_the_fact = false },

  // ARRAY_EXPR_FMT
  { .size = 0,  .after_the_fact = false },

  // OBJECT_EXPR_FMT
  { .size = 0,  .after_the_fact = false },

  // FCN_DEFN_EXPR_FMT
  { .size = 0,  .after_the_fact = false },

  // PREFIX_UNOP_EXPR_FMT
  { .size = 1,  .after_the_fact = false },

  // POSTFIX_UNOP_EXPR_FMT
  { .size = 1,  .after_the_fact = true  },

  // INFIX_BINOP_EXPR_FMT
  { .size = 2,  .after_the_fact = true  },

  // PAREN_EXPR_FMT
  { .size = 1,  .after_the_fact = false },

  // COMP_ACCESS_EXPR_FMT
  { .size = 2,  .after_the_fact = true  },

  // NEW_ARGS_EXPR_FMT
  { .size = 0,  .after_the_fact = false },

  // FCN_CALL_EXPR_FMT
  { .size = 0,  .after_the_fact = true  },

  // CONF_TRIOP_EXPR_FMT
  { .size = 3,  .after_the_fact = true  }
};

const EXPR_TYPE_INFO type_info[NUM_EXPR_TYPES] = {
  // IND_EXPR [indeterminate]
  { .format = INV_EXPR_FMT, .precedence = INVALID_EXPR_PREC },

  // ROOT_EXPR [placeholder at the top of a statement's expr tree]
  { .format = ROOT_EXPR_FMT, .precedence = LEAF_EXPR_PREC },

  // LIT_NUM_EXPR [numeric literal]
  { .format = LEAF_EXPR_FMT, .precedence = LEAF_EXPR_PREC },

  // LIT_STR_EXPR [string literal]
  { .format = LEAF_EXPR_FMT, .precedence = LEAF_EXPR_PREC },

  // SYMBOL_EXPR [string data]
  { .format = LEAF_EXPR_FMT, .precedence = LEAF_EXPR_PREC },

  // DECL_EXPR [string data (similar to SYMBOL but declares a )]
  { .format = VAR_DECL_EXPR_FMT, .precedence = CONTAINER_EXPR_PREC },

  // ARRAY_EXPR [arbitrary list of expressions]
  { .format = ARRAY_EXPR_FMT, .precedence = CONTAINER_EXPR_PREC },

  // OBJECT_EXPR [alternates between symbols and RVALUE types]
  { .format = OBJECT_EXPR_FMT, .precedence = CONTAINER_EXPR_PREC },

  // FUNCTION_EXPR [a function definition]
  { .format = FCN_DEFN_EXPR_FMT, .precedence = CONTAINER_EXPR_PREC },

  // PAREN_EXPR [a)]
  { .format = PAREN_EXPR_FMT, .precedence = 20 },

  // ACCESS_EXPR [a.b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 19 },

  // COMP_ACCESS_EXPR [a[b]]
  { .format = COMP_ACCESS_EXPR_FMT, .precedence = 19 },

  // NEW_ARGS_EXPR [new a (b, ...)]
  { .format = NEW_ARGS_EXPR_FMT, .precedence = 19 },

  // CALL_EXPR [a(...)]
  { .format = FCN_CALL_EXPR_FMT, .precedence = 19 },

  // NEW_EXPR [new a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 18 },

  // POST_INCR_EXPR [a ++]
  { .format = POSTFIX_UNOP_EXPR_FMT, .precedence = 17 },

  // POST_DECR_EXPR [a --]
  { .format = POSTFIX_UNOP_EXPR_FMT, .precedence = 17 },

  // NOT_EXPR [! a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // BITNOT_EXPR [~ a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // POS_EXPR [+ a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // NEG_EXPR [- a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // PRE_INCR_EXPR [++ a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // PRE_DECR_EXPR [-- a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // TYPEOF_EXPR [typdeof a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // VOID_EXPR [void a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // DELETE_EXPR [delete a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 16 },

  // EXP_EXPR [a ** b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 15 },

  // TIMES_EXPR [a * b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 14 },

  // QUOT_EXPR [a / b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 14 },

  // MOD_EXPR [a % b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 14 },

  // PLUS_EXPR [a + b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 13 },

  // MINUS_EXPR [a - b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 13 },

  // BSLEFT_EXPR [a << b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 12 },

  // BSRIGHT_EXPR [a >> b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 12 },

  // UBSRIGHT_EXPR [a >>> b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 12 },

  // LT_EXPR [a < b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // GT_EXPR [a > b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // LET_EXPR [a <= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // GET_EXPR [a >= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // IN_EXPR [a in b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // INSTANCEOF_EXPR [a instanceof b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 11 },

  // EQ_EXPR [a == b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 10 },

  // INEQ_EXPR [a != b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 10 },

  // STRICT_EQ_EXPR [a === b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 10 },

  // STRICT_INEQ_EXPR [a !== b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 10 },

  // BAND_EXPR [a & b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 9 },

  // BXOR_EXPR [a ^ b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 8 },

  // BOR_EXPR [a | b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 7 },

  // LAND_EXPR [a && b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 6 },

  // LOR_EXPR [a || b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 5 },

  // COND_EXPR [a ? b : c]
  { .format = CONF_TRIOP_EXPR_FMT, .precedence = 4 },

  // ASSIGN_EXPR [a = b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_PLUS_EXPR [a += b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_TIMES_EXPR [a *= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_EXP_EXPR [a **= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_QUOT_EXPR [a /= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_MOD_EXPR [a %= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_BSLEFT_EXPR [a <<= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_BSRIGHT_EXPR [a >>= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_UBSRIGHT_EXPR [a >>>= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_BAND_EXPR [a &= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_BXOR_EXPR [a ^= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // ASSIGN_BOR_EXPR [a |= b]
  { .format = INFIX_BINOP_EXPR_FMT, .precedence = 3 },

  // YIELD_EXPR [yield a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 2 },

  // YIELD_STAR_EXPR [yield *a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 2 },

  // SPREAD_EXPR [... a]
  { .format = PREFIX_UNOP_EXPR_FMT, .precedence = 1 },

  // COMMA_EXPR [a, b (valuates to b)]
  { .format =  INFIX_BINOP_EXPR_FMT, .precedence = 0 }
};

// each EXPR_TYPE has an associated
// EXPR_FORMAT which determines its
// internal state machine

// const EXPR_FORMAT expr_type_fmts[NUM_EXPR_TYPES] = {
//   INV_EXPR_FMT,  // IND_EXPR      indeterminate
//   ROOT_EXPR_FMT, // ROOT_EXPR     placeholder at the top of a statement's expr tree
//   LEAF_EXPR_FMT, // LIT_NUM_EXPR  numeric literal
//   LEAF_EXPR_FMT, // LIT_STR_EXPR  string literal
//   LEAF_EXPR_FMT, // SYMBOL_EXPR   string data
//   VAR_DECL_EXPR_FMT, // DECL_EXPR      string data (similar to SYMBOL but declares a )
//   ARRAY_EXPR_FMT,    // ARRAY_EXPR     arbitrary list of expressions
//   OBJECT_EXPR_FMT,   // OBJECT_EXPR    alternates between symbols and RVALUE types
//   FCN_DEFN_EXPR_FMT, // FUNCTION_EXPR  a function definition
//   PAREN_EXPR_FMT,       // PAREN_EXPR         (a)
//   INFIX_BINOP_EXPR_FMT, // ACCESS_EXPR        a.b
//   COMP_ACCESS_EXPR_FMT, // COMP_ACCESS_EXPR   a[b]
//   NEW_ARGS_EXPR_FMT,    // NEW_ARGS_EXPR      new a (b, ...)
//   FCN_CALL_EXPR_FMT,    // CALL_EXPR          a(...)
//   PREFIX_UNOP_EXPR_FMT, // NEW_EXPR           new a
//   POSTFIX_UNOP_EXPR_FMT, // POST_INCR_EXPR     a ++
//   POSTFIX_UNOP_EXPR_FMT, // POST_DECR_EXPR     a --
//   PREFIX_UNOP_EXPR_FMT, // NOT_EXPR           ! a
//   PREFIX_UNOP_EXPR_FMT, // BITNOT_EXPR        ~ a
//   PREFIX_UNOP_EXPR_FMT, // POS_EXPR           + a
//   PREFIX_UNOP_EXPR_FMT, // NEG_EXPR           - a
//   PREFIX_UNOP_EXPR_FMT, // PRE_INCR_EXPR      ++ a
//   PREFIX_UNOP_EXPR_FMT, // PRE_DECR_EXPR      -- a
//   PREFIX_UNOP_EXPR_FMT, // TYPEOF_EXPR        typdeof a
//   PREFIX_UNOP_EXPR_FMT, // VOID_EXPR          void a
//   PREFIX_UNOP_EXPR_FMT, // DELETE_EXPR        delete a
//   INFIX_BINOP_EXPR_FMT, // EXP_EXPR           a ** b
//   INFIX_BINOP_EXPR_FMT, // TIMES_EXPR         a * b
//   INFIX_BINOP_EXPR_FMT, // QUOT_EXPR          a / b
//   INFIX_BINOP_EXPR_FMT, // MOD_EXPR           a % b
//   INFIX_BINOP_EXPR_FMT, // PLUS_EXPR          a + b
//   INFIX_BINOP_EXPR_FMT, // MINUS_EXPR         a - b
//   INFIX_BINOP_EXPR_FMT, // BSLEFT_EXPR        a << b
//   INFIX_BINOP_EXPR_FMT, // BSRIGHT_EXPR       a >> b
//   INFIX_BINOP_EXPR_FMT, // UBSRIGHT_EXPR      a >>> b
//   INFIX_BINOP_EXPR_FMT, // LT_EXPR            a < b
//   INFIX_BINOP_EXPR_FMT, // GT_EXPR            a > b
//   INFIX_BINOP_EXPR_FMT, // LET_EXPR           a <= b
//   INFIX_BINOP_EXPR_FMT, // GET_EXPR           a >= b
//   INFIX_BINOP_EXPR_FMT, // IN_EXPR            a in b
//   INFIX_BINOP_EXPR_FMT, // INSTANCEOF_EXPR    a instanceof b
//   INFIX_BINOP_EXPR_FMT, // EQ_EXPR            a == b
//   INFIX_BINOP_EXPR_FMT, // INEQ_EXPR          a != b
//   INFIX_BINOP_EXPR_FMT, // STRICT_EQ_EXPR     a === b
//   INFIX_BINOP_EXPR_FMT, // STRICT_INEQ_EXPR   a !== b
//   INFIX_BINOP_EXPR_FMT, // BAND_EXPR          a & b
//   INFIX_BINOP_EXPR_FMT, // BXOR_EXPR          a ^ b
//   INFIX_BINOP_EXPR_FMT, // BOR_EXPR           a | b
//   INFIX_BINOP_EXPR_FMT, // LAND_EXPR          a && b
//   INFIX_BINOP_EXPR_FMT, // LOR_EXPR           a || b
//   CONF_TRIOP_EXPR_FMT,  // COND_EXPR          a ? b : c
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_EXPR           a = b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_PLUS_EXPR      a += b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_TIMES_EXPR     a *= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_EXP_EXPR       a **= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_QUOT_EXPR      a /= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_MOD_EXPR       a %= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_BSLEFT_EXPR    a <<= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_BSRIGHT_EXPR   a >>= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_UBSRIGHT_EXPR  a >>>= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_BAND_EXPR      a &= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_BXOR_EXPR      a ^= b
//   INFIX_BINOP_EXPR_FMT, // ASSIGN_BOR_EXPR       a |= b
//   PREFIX_UNOP_EXPR_FMT, // YIELD_EXPR            yield a
//   PREFIX_UNOP_EXPR_FMT, // YIELD_STAR_EXPR       yield *a
//   PREFIX_UNOP_EXPR_FMT, // SPREAD_EXPR           ... a
//   INFIX_BINOP_EXPR_FMT  // COMMA_EXPR            a, b (valuates to b)
// };

// const EXPR_PRECEDENCE opr_prec_info[NUM_EXPR_TYPES] = {
//   INVALID_EXPR_PREC, // IND_EXPR
//   LEAF_EXPR_PREC, // ROOT_EXPR
//   LEAF_EXPR_FMT, // LIT_NUM_EXPR  numeric literal
//   LEAF_EXPR_FMT, // LIT_STR_EXPR  string literal
//   LEAF_EXPR_FMT, // SYMBOL_EXPR   string data
//   CONTAINER_EXPR_PREC, // DECL_EXPR      string data (similar to SYMBOL but declares a )
//   CONTAINER_EXPR_PREC, // ARRAY_EXPR     arbitrary list of expressions
//   CONTAINER_EXPR_PREC, // OBJECT_EXPR    alternates between symbols and RVALUE types
//   CONTAINER_EXPR_PREC, // FUNCTION_EXPR  a function definition
//   20, // PAREN_EXPR         (a)
//   19, // ACCESS_EXPR        a.b
//   19, // COMP_ACCESS_EXPR   a[b]
//   19, // NEW_ARGS_EXPR      new a (b, ...)
//   19, // CALL_EXPR          a(...)
//   18, // NEW_EXPR           new a
//   17, // POST_INCR_EXPR     a ++
//   17, // POST_DECR_EXPR     a --
//   16, // NOT_EXPR           ! a
//   16, // BITNOT_EXPR        ~ a
//   16, // POS_EXPR           + a
//   16, // NEG_EXPR           - a
//   16, // PRE_INCR_EXPR      ++ a
//   16, // PRE_DECR_EXPR      -- a
//   16, // TYPEOF_EXPR        typdeof a
//   16, // VOID_EXPR          void a
//   16, // DELETE_EXPR        delete a
//   15, // EXP_EXPR           a ** b
//   14, // TIMES_EXPR         a * b
//   14, // QUOT_EXPR          a / b
//   14, // MOD_EXPR           a % b
//   13, // PLUS_EXPR          a + b
//   13, // MINUS_EXPR         a - b
//   12, // BSLEFT_EXPR        a << b
//   12, // BSRIGHT_EXPR       a >> b
//   12, // UBSRIGHT_EXPR      a >>> b
//   11, // LT_EXPR            a < b
//   11, // GT_EXPR            a > b
//   11, // LET_EXPR           a <= b
//   11, // GET_EXPR           a >= b
//   11, // IN_EXPR            a in b
//   11, // INSTANCEOF_EXPR    a instanceof b
//   10, // EQ_EXPR            a == b
//   10, // INEQ_EXPR          a != b
//   10, // STRICT_EQ_EXPR     a === b
//   10, // STRICT_INEQ_EXPR   a !== b
//   9,  // BAND_EXPR          a & b
//   8,  // BXOR_EXPR          a ^ b
//   7,  // BOR_EXPR           a | b
//   6,  // LAND_EXPR          a && b
//   5,  // LOR_EXPR           a || b
//   4,  // COND_EXPR          a ? b : c
//   3,  // ASSIGN_EXPR           a = b
//   3,  // ASSIGN_PLUS_EXPR      a += b
//   3,  // ASSIGN_TIMES_EXPR     a *= b
//   3,  // ASSIGN_EXP_EXPR       a **= b
//   3,  // ASSIGN_QUOT_EXPR      a /= b
//   3,  // ASSIGN_MOD_EXPR       a %= b
//   3,  // ASSIGN_BSLEFT_EXPR    a <<= b
//   3,  // ASSIGN_BSRIGHT_EXPR   a >>= b
//   3,  // ASSIGN_UBSRIGHT_EXPR  a >>>= b
//   3,  // ASSIGN_BAND_EXPR      a &= b
//   3,  // ASSIGN_BXOR_EXPR      a ^= b
//   3,  // ASSIGN_BOR_EXPR       a |= b
//   2,  // YIELD_EXPR            yield a
//   2,  // YIELD_STAR_EXPR       yield *a
//   1,  // SPREAD_EXPR           ... a
//   0   // COMMA_EXPR            a, b (valuates to b)
// };
