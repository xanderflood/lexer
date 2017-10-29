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
  IND_EXPR,       // indeterminate

  ROOT_EXPR,      // placeholder at the top of a statement's expr tree

  // nouns (leaves)
  LIT_NUM_EXPR,   // numeric literal
  LIT_STR_EXPR,   // string literal
  SYMBOL_EXPR,    // string data
  DECL_EXPR,      // string data (similar to SYMBOL but declares a )

  // operations (internal nodes)
  PAREN_EXPR,        // 1 expr
  ASSIGN_EXPR,       // 2 arbitrary expressions
  ASSIGN_PLUS_EXPR,  // 2 arbitrary expressions
  ASSIGN_TIMES_EXPR, // 2 arbitrary expressions
  CALL_EXPR,         // children are arbitrary expression followed by a list of arguments
  FUNCTION_EXPR,     // arbitrary list of statements
  ARRAY_EXPR,        // arbitrary list of expressions
  OBJECT_EXPR,       // alternates between symbols and RVALUE types
  PLUS_EXPR,         // 2 arbitrary expressions
  POST_INCR_EXPR,    // 1 arbitrary expression
  PRE_INCR_EXPR,     // 1 arbitrary expression
  TIMES_EXPR,        // 2 arbitrary expressions
  EQUALS_EXPR,       // 2 arbitrary expressions
  LT_EXPR,           // 2 arbitrary expressions
  GT_EXPR,           // 2 arbitrary expressions
  LET_EXPR,          // 2 arbitrary expressions
  GET_EXPR,          // 2 arbitrary expressions

  COMMA_EXPR         // n arbitrary expressions - evaluates each and then returns the last
} expression_type;
#define num_expr_types 24

#define EXPR_TYPE_VERB(et) ((et >= PAREN_EXPR) && (et <= COMMA_EXPR))
#define EXPR_TYPE_NOUN(et) (et == LIT_NUM_EXPR || et == LIT_STR_EXPR || et == SYMBOL_EXPR || et == DECL_EXPR)
#define EXPR_TYPE_INTERNAL_NODE(expr) (EXPR_TYPE_VERB((expr)->type) || (expr)->type == ROOT_EXPR)

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
  signed char state;
};

#define INVALID_EXPR_STATE(expr)   (!!(expr))
#define TERMINAL_EXPR_STATE(expr)  ((expr)->state < 0)
#define FINISHED_EXPR_STATE(expr)  (TERMINAL_EXPR_STATE(expr) && !EXPECTANT_EXPR_STATE(expr))
#define EXPECTANT_EXPR_STATE(expr) (!((expr)->state % 2))
#define SET_EXPR_TYPE(expr, _type) do { ((expr)->type = _type); init_state(expr); } while (0)

// alloc.c
JS_STMT *init_statement();
JS_EXPR *init_expression();
void destroy_statement(JS_STMT *stmt);
void destroy_expression(JS_EXPR *expr);
void destroy_statement_list(JS_STMTS stmt);
void destroy_expression_list(JS_EXPRS expr);

// tree.c
void    push_statement(JS_STMTS *list, JS_STMT *data);
JS_STMT *pop_statement(JS_STMTS *list);
JS_EXPR *pop_expression(JS_EXPRS *list);
void         add_child_expression(JS_EXPR *parent, JS_EXPR *child);
void add_postfix_child_expression(JS_EXPR *expr,  JS_EXPR *new);

// read.c
int read_statement(JS_STMT *stmt);
int read_block(JS_EXPR *block);

#endif
