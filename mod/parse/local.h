#include "parse.h"
#include "lex.h"

// tree.c
void push_expression(JS_EXPRS *list, JS_EXPR *data);

// syntax.c
int update_state(JS_EXPR *parent, TOKEN *new);
void init_state(JS_EXPR *expr);

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
} expression_format;

#define num_expression_formats 15
const bool expr_type_after_the_fact[num_expression_formats];
const expression_format expr_type_fmts[num_expr_types];

typedef enum {
  INVALID_EXPR_PREC = -1,
  LEAF_EXPR_PREC = -2,
  CONTAINER_EXPR_PREC = -3
} expression_precedence;

const expression_precedence opr_prec_info[num_expr_types];

expression_type interpret_operator_token_as_noun(TOKEN *tok);
expression_type interpret_operator_token_as_verb(TOKEN *tok);
