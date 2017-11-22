#ifndef PARSE_H
#define PARSE_H

#include "shared.h"
#include "lex.h"

#include "parse/enums.h"

#define EXPR_TYPE_VERB(et) ((et >= PAREN_EXPR) && (et <= COMMA_EXPR))
#define EXPR_TYPE_NOUN(et) (et == LIT_NUM_EXPR || et == LIT_STR_EXPR || et == SYMBOL_EXPR || et == DECL_EXPR)
#define EXPR_TYPE_INTERNAL_NODE(expr) (EXPR_TYPE_VERB((expr)->type) || (expr)->type == ROOT_EXPR)

#define TERMINAL_EXPR_STATE(expr)    ((expr)->state < 0)
#define INVALID_EXPR_STATE(expr)     ((expr)->state == 0)
#define EXPECTANT_EXPR_STATE(expr)   ((expr)->state % 2 == 0)
#define VIS_CHILD_EXPR_STATE(expr)   ((expr)->state % 3 == 0)
#define EXPECTING_SIGNAL_STATE(expr) ((expr)->state % 5 == 0)
#define FINISHED_EXPR_STATE(expr)    (TERMINAL_EXPR_STATE(expr) && !EXPECTANT_EXPR_STATE(expr) && !EXPECTING_SIGNAL_STATE(expr))

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
  STMT_TYPE type;

  JS_STMTS children;
  JS_EXPRS expressions;
};

struct JS_EXPR {
  EXPR_TYPE type;

  void *data;
  JS_EXPRS children;
  JS_EXPR *parent;

  // initial state is 1
  // terminal states are negative
  // 0 is a failed state
  // see `EXPR_STATE` in syntax.h for further details
  EXPR_STATE state;
  int child_count;
};

// alloc.c
JS_STMT *init_statement();
JS_EXPR *init_expression(EXPR_TYPE type);
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
int read_statement(TOKEN_STREAM *ts, JS_STMT *stmt);
int read_block(JS_EXPR *block);

// print.c
void print_expression_tree(JS_EXPR *expr);

#endif
