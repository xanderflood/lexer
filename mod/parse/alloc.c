#include "local.h"

JS_STMT *init_statement() {
  JS_EXPR *root;
  JS_STMT *stmt = (JS_STMT*)malloc(sizeof(JS_STMT));
  stmt->type = IND_STMT;
  stmt->children = NULL;
  stmt->expressions = NULL;

  // Add a root expression
  root = init_expression(ROOT_EXPR);
  push_expression(&stmt->expressions, root);

  return stmt;
}

JS_EXPR *init_expression(expression_type t) {
  JS_EXPR *expr = (JS_EXPR*)malloc(sizeof(JS_EXPR));
  expr->children = NULL;
  expr->parent   = NULL;
  expr->state    = 0;

  expr->type = t;
  init_state(expr);

  return expr;
}

void destroy_statement(JS_STMT *stmt) {
  free(stmt);
}

void destroy_expression(JS_EXPR *expr) {
  free(expr);
}

void destroy_statement_list(JS_STMTS stmt) {
  JS_STMTS last;

  do {
    last = stmt;
    stmt = stmt->next;
    free(last);
  } while(stmt->next);
}

void destroy_expression_list(JS_EXPRS expr) {
  JS_EXPRS last;

  do {
    last = expr;
    expr = expr->next;
    free(last);
  } while(expr->next);
}
