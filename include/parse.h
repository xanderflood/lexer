#ifndef PARSE_H
#define PARSE_H

#include "shared.h"

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
} statement_type;

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
} expression_type;
#define num_expr_types 67

#define EXPR_TYPE_VERB(et) ((et >= PAREN_EXPR) && (et <= COMMA_EXPR))
#define EXPR_TYPE_NOUN(et) (et == LIT_NUM_EXPR || et == LIT_STR_EXPR || et == SYMBOL_EXPR || et == DECL_EXPR)
#define EXPR_TYPE_INTERNAL_NODE(expr) (EXPR_TYPE_VERB((expr)->type) || (expr)->type == ROOT_EXPR)

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
} expression_state;

#define TERMINAL_EXPR_STATE(expr)    ((expr)->state < 0)
#define INVALID_EXPR_STATE(expr)     ((expr)->state == 0)
#define EXPECTANT_EXPR_STATE(expr)   ((expr)->state % 2 == 0)
#define VIS_CHILD_EXPR_STATE(expr)   ((expr)->state % 3 == 0)
#define EXPECTING_SIGNAL_STATE(expr) ((expr)->state % 5 == 0)
#define FINISHED_EXPR_STATE(expr)    (TERMINAL_EXPR_STATE(expr) && !EXPECTANT_EXPR_STATE(expr) && !EXPECTING_SIGNAL_STATE(expr))

// a size of 0 means variadic
const char expression_type_sizes[num_expr_types];

// These structs are connected in complicated ways,
// so we have to create the names before describing
// the contents
typedef struct EXPR_ITEM EXPR_ITEM;
typedef struct STMT_ITEM STMT_ITEM;
typedef struct JS_EXPR JS_EXPR;
typedef struct JS_STMT JS_STMT;

// linked lists of JS_EXPRS and JS_STMTS
typedef EXPR_ITEM* JS_EXPRS;
typedef STMT_ITEM* JS_STMTS;
struct EXPR_ITEM { EXPR_ITEM *next; JS_EXPR *data; };
struct STMT_ITEM { STMT_ITEM *next; JS_STMT *data; };

// the things themselves
struct JS_STMT {
  statement_type type;

  JS_STMTS children;
  JS_EXPRS expressions;
};

struct JS_EXPR {
  expression_type type;

  void *data;
  JS_EXPRS children;
  JS_EXPR *parent;

  // TODO:
  // initial state is 1
  // terminal states are negative
  // 0 is a failed state
  expression_state state;
  int child_count;
};

// alloc.c
JS_STMT *init_statement();
JS_EXPR *init_expression(expression_type type);
void destroy_statement(JS_STMT *stmt);
void destroy_expression(JS_EXPR *expr);
void destroy_statement_list(JS_STMTS stmt);
void destroy_expression_list(JS_EXPRS expr);

// tree.c
void    push_statement(JS_STMTS *list, JS_STMT *data);
JS_STMT *pop_statement(JS_STMTS *list);
JS_EXPR *pop_first_child(JS_EXPR *parent);
void         add_child_expression(JS_EXPR *parent, JS_EXPR *child);
void add_postfix_child_expression(JS_EXPR *expr,   JS_EXPR *new);

// read.c
int read_statement(JS_STMT *stmt);
int read_block(JS_EXPR *block);

#endif
