#include "parse.h"

// TODO: write a `stack.h` and use it to refactor JS_LISTS, JS_EXPRS and TOKEN_STREAM all

void push_statement(JS_STMTS *list, JS_STMT *data) {
  JS_STMTS s_list = (JS_STMTS)malloc(sizeof(STMT_ITEM));
  s_list->next = *list;
  s_list->data = data;

  *list = s_list;
}

JS_STMT *pop_statement(JS_STMTS *list) {
  JS_STMT *ret;
  JS_STMTS old;

  if (!*list)
    return NULL;

  ret   = (*list)->data;
  old   = *list;
  *list = (*list)->next;
  free(old);

  return ret;
}

void push_expression(JS_EXPRS *list, JS_EXPR *data) {
  JS_EXPRS e_list = (JS_EXPRS)malloc(sizeof(EXPR_ITEM));
  e_list->next = *list;
  e_list->data = data;

  *list = e_list;
}

JS_EXPR *pop_expression(JS_EXPRS *list) {
  JS_EXPR *ret;
  JS_EXPRS old;

  if (!*list)
    return NULL;

  ret   = (*list)->data;
  old   = *list;
  *list = (*list)->next;
  free(old);

  return ret;
}

void push_postfix(JS_EXPR *expr, JS_EXPR *new) {
  JS_EXPR *child;

  child = pop_expression(&expr->children);
  push_expression(&new->children, child);
  push_expression(&expr->children, new);
}
