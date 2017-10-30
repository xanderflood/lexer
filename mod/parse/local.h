#include "parse.h"
#include "lex.h"

// tree.c
void push_expression(JS_EXPRS *list, JS_EXPR *data);

// syntax.c
int update_state(JS_EXPR *parent, TOKEN *new);
void init_state(JS_EXPR *expr);

typedef enum {
  INVALID_EXPR_PREC = -1,
  LEAF_EXPR_PREC = -2,
  CONTAINER_EXPR_PREC = -3
} expression_precedence;

const expression_precedence opr_prec_info[num_expr_types];
