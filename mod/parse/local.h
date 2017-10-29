#include "parse.h"
#include "lex.h"

// tree.c
void push_expression(JS_EXPRS *list, JS_EXPR *data);

// syntax.c
int update_state(JS_EXPR *parent, TOKEN *new);
void init_state(JS_EXPR *expr);
