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
  ASSIGN_EXPR,       // two arbitrary expressions
  ASSIGN_PLUS_EXPR,  // two arbitrary expressions
  ASSIGN_TIMES_EXPR, // two arbitrary expressions
  CALL_EXPR,         // children are arbitrary expression followed by a list of arguments
  FUNCTION_EXPR,     // arbitrary list of statements
  ARRAY_EXPR,        // arbitrary list of expressions
  OBJECT_EXPR,       // alternates between symbols and RVALUE types
  PLUS_EXPR,         // two arbitrary expressions
  POST_INCR_EXPR,    // one arbitrary expression
  PRE_INCR_EXPR,     // one arbitrary expression
  TIMES_EXPR,        // two arbitrary expressions
  EQUALS_EXPR,       // two arbitrary expressions
  LT_EXPR,           // two arbitrary expressions
  GT_EXPR,           // two arbitrary expressions
  LET_EXPR,          // two arbitrary expressions
  GET_EXPR,          // two arbitrary expressions

  PAREN_EXPR         // not a real expression, but used while building to keep track of operator precedence
} expression_type;

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
  bool unfinished;
};

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
void    push_expression(JS_EXPRS *list, JS_EXPR *data);
JS_EXPR *pop_expression(JS_EXPRS *list);
void    push_postfix(JS_EXPR *expr, JS_EXPR *new);

// read.c
int read_statement(JS_STMT *stmt);
int read_block(JS_EXPR *block);

#endif
