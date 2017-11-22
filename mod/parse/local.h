#ifndef PARSE_LOCAL_H
#define PARSE_LOCAL_H

#include "parse.h"
#include "lex.h"

// tree.c
void push_expression(JS_EXPRS *list, JS_EXPR *data);

// expressions.c
int update_state(JS_EXPR *parent, TOKEN *new);
void init_state(JS_EXPR *expr);

EXPR_TYPE interpret_token_as_noun(TOKEN *tok);
EXPR_TYPE interpret_token_as_verb(TOKEN *tok);

// syntax.c
typedef struct {
  // the internal state machine for expressions of this type
  EXPR_FORMAT format;

  // where to position this expression along the frontier of active ones
  EXPR_PRECEDENCE precedence;
} EXPR_TYPE_INFO;

typedef struct {
  // how many children does do expressions of this format have
  // 0 means variadic, -1 means none
  int size;

  // are expressions of this type created after their first child?
  bool after_the_fact;
} EXPR_FORMAT_INFO;

const EXPR_TYPE_INFO type_info[NUM_EXPR_TYPES];
const EXPR_FORMAT_INFO format_info[NUM_EXPR_FORMATS];

#endif