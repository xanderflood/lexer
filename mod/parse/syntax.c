#include "local.h"
#include "string.h"

// each expression_type has an associated
// expression_format which determines its
// internal state machine

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

  INFIX_BINOP_EXPR_FMT  // COMMA_EXPR            a, b (valuates to b)
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

  // TODO: implement

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

// Tells whether or not an operator expression will be
// built *after* the first child expression it receives
// TODO: Any node that becomes a child of a node created *after*
//       it is declared *finished* and becomes invisible to the
//       AST building process.
const bool expr_type_after_the_fact[num_expression_formats] = {
  false, // LEAF_EXPR_FMT

  false, // ROOT_EXPR_FMT

  false, // VAR_DECL_EXPR_FMT
  false, // ARRAY_EXPR_FMT
  false, // OBJECT_EXPR_FMT
  false, // FCN_DEFN_EXPR_FMT

  false, // PREFIX_UNOP_EXPR_FMT
  true,  // POSTFIX_UNOP_EXPR_FMT
  true,  // INFIX_BINOP_EXPR_FMT

  false, // PAREN_EXPR_FMT
  true,  // COMP_ACCESS_EXPR_FMT
  false, // NEW_ARGS_EXPR_FMT
  true,  // FCN_CALL_EXPR_FMT

  true   // CONF_TRIOP_EXPR_FMT
};

const expression_precedence opr_prec_info[num_expr_types] = {
  INVALID_EXPR_PREC, // IND_EXPR

  LEAF_EXPR_PREC, // ROOT_EXPR

  LEAF_EXPR_FMT, // LIT_NUM_EXPR  numeric literal
  LEAF_EXPR_FMT, // LIT_STR_EXPR  string literal
  LEAF_EXPR_FMT, // SYMBOL_EXPR   string data

  CONTAINER_EXPR_PREC, // DECL_EXPR      string data (similar to SYMBOL but declares a )
  CONTAINER_EXPR_PREC, // ARRAY_EXPR     arbitrary list of expressions
  CONTAINER_EXPR_PREC, // OBJECT_EXPR    alternates between symbols and RVALUE types
  CONTAINER_EXPR_PREC, // FUNCTION_EXPR  a function definition

  20, // PAREN_EXPR         (a)
  19, // ACCESS_EXPR        a.b
  19, // COMP_ACCESS_EXPR   a[b]
  19, // NEW_ARGS_EXPR      new a (b, ...)
  19, // CALL_EXPR          a(...)
  18, // NEW_EXPR           new a
  17, // POST_INCR_EXPR     a ++
  17, // POST_DECR_EXPR     a --
  16, // NOT_EXPR           ! a
  16, // BITNOT_EXPR        ~ a
  16, // POS_EXPR           + a
  16, // NEG_EXPR           - a
  16, // PRE_INCR_EXPR      ++ a
  16, // PRE_DECR_EXPR      -- a
  16, // TYPEOF_EXPR        typdeof a
  16, // VOID_EXPR          void a
  16, // DELETE_EXPR        delete a
  15, // EXP_EXPR           a ** b
  14, // TIMES_EXPR         a * b
  14, // QUOT_EXPR          a / b
  14, // MOD_EXPR           a % b
  13, // PLUS_EXPR          a + b
  13, // MINUS_EXPR         a - b
  12, // BSLEFT_EXPR        a << b
  12, // BSRIGHT_EXPR       a >> b
  12, // UBSRIGHT_EXPR      a >>> b
  11, // LT_EXPR            a < b
  11, // GT_EXPR            a > b
  11, // LET_EXPR           a <= b
  11, // GET_EXPR           a >= b
  11, // IN_EXPR            a in b
  11, // INSTANCEOF_EXPR    a instanceof b
  10, // EQ_EXPR            a == b
  10, // INEQ_EXPR          a != b
  10, // STRICT_EQ_EXPR     a === b
  10, // STRICT_INEQ_EXPR   a !== b
  9,  // BAND_EXPR          a & b
  8,  // BXOR_EXPR          a ^ b
  7,  // BOR_EXPR           a | b
  6,  // LAND_EXPR          a && b
  5,  // LOR_EXPR           a || b
  4,  // COND_EXPR          a ? b : c
  3,  // ASSIGN_EXPR           a = b
  3,  // ASSIGN_PLUS_EXPR      a += b
  3,  // ASSIGN_TIMES_EXPR     a *= b
  3,  // ASSIGN_EXP_EXPR       a **= b
  3,  // ASSIGN_QUOT_EXPR      a /= b
  3,  // ASSIGN_MOD_EXPR       a %= b
  3,  // ASSIGN_BSLEFT_EXPR    a <<= b
  3,  // ASSIGN_BSRIGHT_EXPR   a >>= b
  3,  // ASSIGN_UBSRIGHT_EXPR  a >>>= b
  3,  // ASSIGN_BAND_EXPR      a &= b
  3,  // ASSIGN_BXOR_EXPR      a ^= b
  3,  // ASSIGN_BOR_EXPR       a |= b
  2,  // YIELD_EXPR            yield a
  2,  // YIELD_STAR_EXPR       yield *a
  1,  // SPREAD_EXPR           ... a
  0   // COMMA_EXPR            a, b (valuates to b)
};

expression_type interpret_token_as_verb(TOKEN *tok) {
  if (tok->type == NUM_TOK)
    return LIT_NUM_EXPR;     // numeric literal
  else if (tok->type == LIT_TOK)
    return LIT_STR_EXPR;     // string literal
  else if (tok->type == SYM_TOK)
    return SYMBOL_EXPR;      // string data
  else if (tok->type == OPR_TOK && strcmp(tok->s, "var"))
    return DECL_EXPR;        // string data (similar to SYMBOL but declares a )
  else if (tok->type == PNC_TOK && strcmp(tok->s, "["))
    return ARRAY_EXPR;       // arbitrary list of expressions
  else if (tok->type == PNC_TOK && strcmp(tok->s, "{"))
    return OBJECT_EXPR;      // alternates between symbols and RVALUE types
  else if (tok->type == OPR_TOK && strcmp(tok->s, "function"))
    return FUNCTION_EXPR;    // a function definition
  else if (tok->type == PNC_TOK && strcmp(tok->s, "("))
    return PAREN_EXPR;       // (a)
  else if (tok->type == OPR_TOK && strcmp(tok->s, "new"))
    return NEW_ARGS_EXPR;    // new a (b, ...)
                       // TODO: new a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!"))
    return NOT_EXPR;         // ! a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "~"))
    return BITNOT_EXPR;      // ~ a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+"))
    return POS_EXPR;         // + a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "-"))
    return NEG_EXPR;         // - a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "++"))
    return PRE_INCR_EXPR;    // ++ a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "--"))
    return PRE_DECR_EXPR;    // -- a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "typdeof"))
    return TYPEOF_EXPR;      // typdeof a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "void"))
    return VOID_EXPR;        // void a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "delete"))
    return DELETE_EXPR;      // delete a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "yield"))
    return YIELD_EXPR;       // yield a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "yield*"))
    return YIELD_STAR_EXPR;  // yield *a
  else if (tok->type == PNC_TOK && strcmp(tok->s, "..."))
    return SPREAD_EXPR;      // ... a
  else
    return IND_EXPR;
}

expression_type interpret_token_as_noun(TOKEN *tok) {
  assert(tok->type == OPR_TOK || tok->type == PNC_TOK);

  if (tok->type == OPR_TOK && strcmp(tok->s, "."))
    return ACCESS_EXPR;          // a.b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "b"))
    return COMP_ACCESS_EXPR;      // a[b]
  else if (tok->type == OPR_TOK && strcmp(tok->s, "..."))
    return CALL_EXPR;             // a(...)
  else if (tok->type == OPR_TOK && strcmp(tok->s, "++"))
    return POST_INCR_EXPR;        // a ++
  else if (tok->type == OPR_TOK && strcmp(tok->s, "--"))
    return POST_DECR_EXPR;        // a --
  else if (tok->type == OPR_TOK && strcmp(tok->s, "**"))
    return EXP_EXPR;              // a ** b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "*"))
    return TIMES_EXPR;            // a * b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "/"))
    return QUOT_EXPR;             // a / b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "%"))
    return MOD_EXPR;              // a % b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+"))
    return PLUS_EXPR;             // a + b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "-"))
    return MINUS_EXPR;            // a - b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<<"))
    return BSLEFT_EXPR;           // a << b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>"))
    return BSRIGHT_EXPR;          // a >> b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>>"))
    return UBSRIGHT_EXPR;         // a >>> b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<"))
    return LT_EXPR;               // a < b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">"))
    return GT_EXPR;               // a > b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<="))
    return LET_EXPR;              // a <= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">="))
    return GET_EXPR;              // a >= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "in"))
    return IN_EXPR;               // a in b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "instanceof"))
    return INSTANCEOF_EXPR;       // a instanceof b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "=="))
    return EQ_EXPR;               // a == b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!="))
    return INEQ_EXPR;             // a != b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "==="))
    return STRICT_EQ_EXPR;        // a === b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!=="))
    return STRICT_INEQ_EXPR;      // a !== b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&"))
    return BAND_EXPR;             // a & b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "^"))
    return BXOR_EXPR;             // a ^ b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "|"))
    return BOR_EXPR;              // a | b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&&"))
    return LAND_EXPR;             // a && b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "||"))
    return LOR_EXPR;              // a || b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "?"))
    return COND_EXPR;             // a ? b : c
  else if (tok->type == OPR_TOK && strcmp(tok->s, "="))
    return ASSIGN_EXPR;           // a = b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+="))
    return ASSIGN_PLUS_EXPR;      // a += b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "*="))
    return ASSIGN_TIMES_EXPR;     // a *= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "**="))
    return ASSIGN_EXP_EXPR;       // a **= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "/="))
    return ASSIGN_QUOT_EXPR;      // a /= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "%="))
    return ASSIGN_MOD_EXPR;       // a %= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<<="))
    return ASSIGN_BSLEFT_EXPR;    // a <<= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>="))
    return ASSIGN_BSRIGHT_EXPR;   // a >>= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>>="))
    return ASSIGN_UBSRIGHT_EXPR;  // a >>>= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&="))
    return ASSIGN_BAND_EXPR;      // a &= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "^="))
    return ASSIGN_BXOR_EXPR;      // a ^= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "|="))
    return ASSIGN_BOR_EXPR;       // a |= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ","))
    return COMMA_EXPR;            // a, b (valuates to b)
  else
    return IND_EXPR;
}
