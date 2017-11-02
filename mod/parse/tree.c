#include "local.h"

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

static JS_EXPR *pop_expression(JS_EXPRS *list) {
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

JS_EXPR *pop_first_child(JS_EXPR *parent) {
  assert(VIS_CHILD_EXPR_STATE(parent));

  parent->child_count --;
  return pop_expression(&parent->children);
}

void push_expression(JS_EXPRS *list, JS_EXPR *data) {
  JS_EXPRS e_list = (JS_EXPRS)malloc(sizeof(EXPR_ITEM));
  e_list->next = *list;
  e_list->data = data;

  *list = e_list;
}

void add_child_expression(JS_EXPR *parent, JS_EXPR *child) {
  // TODO: confirm that the *current* first child of parent is in a terminal state

  parent->child_count ++;
  update_state(parent, NULL);
  push_expression(&parent->children, child);
}

void add_postfix_child_expression(JS_EXPR *expr, JS_EXPR *new) {
  JS_EXPR *child;
  child = pop_first_child(expr);

  // can't subjugate an unfinished child
  assert(TERMINAL_EXPR_STATE(child));

  add_child_expression(new, child);
  add_child_expression(expr, new);
}
