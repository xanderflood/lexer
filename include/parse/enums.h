typedef enum {
  IND_STMT,          // indeterminate
  EXPR_STMT,         // a statement consisting of just one expression to evaluate
  IF_STMT,           // 1 expr  followed by one or two blocks
  FOR_STMT,          // for (three child expressions followed by a block)
  FOR_IN_STMT,       // for (a in b) (two child expressions and then a block)
  FUNCTION_STMT,     // function(a,...,z) (any number of child expressions ending with a block)
  WHILE_STMT,        // while (...) (one expressions and then a block)
  DO_WHILE_STMT,     // do {...} while (...) (one block followed by an expression)
  SWITCH_STMT,       // ??
  WITH_STMT,         // one expression then a block
  TRY_CATCH_STMT,    // block, possible expression, then another block
  COMPOUND_STMT
} STMT_TYPE;

typedef enum {
  IND_EXPR,     // indeterminate

  ROOT_EXPR,    // placeholder at the top of a statement's expr tree

  // noun literals (leaves)
  LIT_NUM_EXPR, // numeric literal
  LIT_STR_EXPR, // string literal
  SYMBOL_EXPR,  // string data

  // noun expressions
  DECL_EXPR,     // string data (similar to SYMBOL but declares a )
  ARRAY_EXPR,    // arbitrary list of expressions
  OBJECT_EXPR,   // alternates between symbols and RVALUE types
  FUNCTION_EXPR, // a function definition

  // operations (internal nodes)
  PAREN_EXPR,        // (a)
  ACCESS_EXPR,       // a.b
  COMP_ACCESS_EXPR,  // a[b]
  NEW_ARGS_EXPR,     // new a (...)
  CALL_EXPR,         // a (...)

  NEW_EXPR,          // new a

  POST_INCR_EXPR,    // a ++
  POST_DECR_EXPR,    // a --

  // prefix unary operations
  NOT_EXPR,          // ! a
  BITNOT_EXPR,       // ~ a
  POS_EXPR,          // + a
  NEG_EXPR,          // - a
  PRE_INCR_EXPR,     // ++ a
  PRE_DECR_EXPR,     // -- a
  TYPEOF_EXPR,       // typdeof a
  VOID_EXPR,         // void a
  DELETE_EXPR,       // delete a

  EXP_EXPR,          // a ** b

  TIMES_EXPR,        // a * b
  QUOT_EXPR,         // a / b
  MOD_EXPR,          // a % b

  PLUS_EXPR,         // a + b
  MINUS_EXPR,        // a - b

  BSLEFT_EXPR,       // a << b
  BSRIGHT_EXPR,      // a >> b
  UBSRIGHT_EXPR,     // a >>> b

  LT_EXPR,           // a < b
  GT_EXPR,           // a > b
  LET_EXPR,          // a <= b
  GET_EXPR,          // a >= b
  IN_EXPR,           // a in b
  INSTANCEOF_EXPR,   // a instanceof b

  EQ_EXPR,           // a == b
  INEQ_EXPR,         // a != b
  STRICT_EQ_EXPR,    // a === b
  STRICT_INEQ_EXPR,  // a !== b

  BAND_EXPR,         // a & b

  BXOR_EXPR,         // a ^ b

  BOR_EXPR,          // a | b

  LAND_EXPR,         // a && b

  LOR_EXPR,          // a || b

  COND_EXPR,         // a ? b : c

  ASSIGN_EXPR,          // a = b
  ASSIGN_PLUS_EXPR,     // a += b
  ASSIGN_TIMES_EXPR,    // a *= b
  ASSIGN_EXP_EXPR,      // a **= b
  ASSIGN_QUOT_EXPR,     // a /= b
  ASSIGN_MOD_EXPR,      // a %= b
  ASSIGN_BSLEFT_EXPR,   // a <<= b
  ASSIGN_BSRIGHT_EXPR,  // a >>= b
  ASSIGN_UBSRIGHT_EXPR, // a >>>= b
  ASSIGN_BAND_EXPR,     // a &= b
  ASSIGN_BXOR_EXPR,     // a ^= b
  ASSIGN_BOR_EXPR,      // a |= b

  YIELD_EXPR,           // yield a
  YIELD_STAR_EXPR,      // yield *a

  SPREAD_EXPR,          // ... a

  COMMA_EXPR            // a, b (valuates to b)
} EXPR_TYPE;
#define NUM_EXPR_TYPES 67

typedef enum {
  INV_EXPR_FMT,
  LEAF_EXPR_FMT,
  ROOT_EXPR_FMT,

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
} EXPR_FORMAT;
#define NUM_EXPR_FORMATS 15

typedef enum {
  INVALID_EXPR_PREC = -1,
  LEAF_EXPR_PREC = -2,
  CONTAINER_EXPR_PREC = -3
} EXPR_PRECEDENCE;

// 0 is invalid
// negative states are potentially terminal
// multiples of 2 will accept a new child
// multiples of 3 have a visible child
// multiples of 5 will accept a signal token
typedef enum {
  INVALID_EXST = 0,

  FINISHED_EXST = -1,   // totally finished
  FINISHED_VIS_EXST = -3,   // finished, but it's leading child is still visible and open to manipulation
  TERM_C_EXST = -2,         // will accept a child
  TERM_S_EXST = -5,         // will accept a signal token
  // TERM_CV_EXST = -6,     // NEVER USED in JS (In Ruby, for instance, this is used to do paren-less function calls)
  // TERM_CS_EXST = -10,    // NEVER USED

  // NTERM_EXST = 1,    // impossible since it won't accept anything
  NTERM_C_EXST = 2,     // will accept a child
  NTERM_V_EXST = 3,     // has a visible child
  NTERM_S_EXST = 5,     // will accept a signal token
  // NTERM_CV_EXST = 6, // NEVER USED in JS
  NTERM_CS_EXST = 10,   // will accept child or signal
  NTERM_SV_EXST = 15,   // has a visible child and will accept a signal token
} EXPR_STATE;
