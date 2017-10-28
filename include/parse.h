#ifndef PARSE_H
#define PARSE_H

#include "shared.h"

// NOTE: It seems to be typical to consider a block
// as a type of statment rather than as an expression.
// This makes sense because a block doesn't produce a
// value, but in my model it is easier to simply consider
// it as a type of expression that is neither an LVALUE
// nor an RVALUE, because my expressions have a list of
// things, and some of them have to be expressions and
// some of them have to be blocks, so I may as well unify
// those.

typedef enum {
  IND_STMT,          // indeterminate
  ASSIGN_STMT,       // 2 exprs linked by  ='s (always LVALUE then RVALUE)
  ASSIGN_PLUS_STMT,  // 2 exprs linked by *='s (always LVALUE then RVALUE)
  ASSIGN_TIMES_STMT, // 2 exprs linked by +='s (always LVALUE then RVALUE)
  IF_STMT,           // 1 expr  followed by one or two blocks
  FOR_STMT,          // for (three child expressions followed by a block)
  FOR_IN_STMT,       // for (a in b) (two child expressions and then a block)
  FUNCTION_STMT,     // function(a,...,z) (any number of child expressions ending with a block)
  WHILE_STMT,        // while (...) (one expressions and then a block)
  DO_WHILE_STMT,     // do {...} while (...) (one block followed by an expression)
  SWITCH_STMT,       // ??
  WITH_STMT,         // one expression then a block
  TRY_CATCH_STMT     // block, possible expression, then another block
} statement_type;

typedef enum {
  IND_EXPR,       // indeterminate
  LIT_NUM_EXPR,   // numeric literal
  LIT_STR_EXPR,   // string literal
  CALL_EXPR,      // children are arbitrary expression followed by a list of arguments
  SYMBOL_EXPR,    // string data
  DECL_EXPR,      // string data (similar to SYMBOL but declares a )
  BLOCK_EXPR,     // arbitrary list of statements
  ARRAY_EXPR,     // arbitrary list of expressions
  OBJECT_EXPR,    // alternates between symbols and RVALUE types
  PLUS_EXPR,      // two arbitrary expressions
  POST_INCR_EXPR, // one arbitrary expression
  PRE_INCR_EXPR,  // one arbitrary expression
  TIMES_EXPR,     // two arbitrary expressions
  EQUALS_EXPR,    // two arbitrary expressions
  LT_EXPR,        // two arbitrary expressions
  GT_EXPR,        // two arbitrary expressions
  LET_EXPR,       // two arbitrary expressions
  GET_EXPR        // two arbitrary expressions
} expression_type;

typedef struct EXPR_ITEM EXPR_ITEM;
typedef struct STMT_ITEM STMT_ITEM;

typedef EXPR_ITEM* JS_EXPRS;
typedef STMT_ITEM* JS_STMTS;

typedef struct {
  statement_type type;

  JS_EXPRS *children;
} JS_STMT;

typedef struct {
  expression_type type;

  void *data;
  JS_EXPRS *children;
  JS_STMTS *statements;
} JS_EXPR;

struct EXPR_ITEM {
  EXPR_ITEM *next;
  JS_EXPR *data;
};

struct STMT_ITEM {
  STMT_ITEM *next;
  JS_STMT *data;
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

// read.c
int read_statement(JS_STMT *stmt);
int read_block(JS_EXPR *block);

#endif
