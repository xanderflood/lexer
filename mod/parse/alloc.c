#include "parse.h"

JS_STMT *init_statement() {
  JS_STMT *stmt = (JS_STMT*)malloc(sizeof(JS_STMT));
  stmt->type = IND_STMT;
  stmt->children = NULL;
  stmt->statements = NULL;

  return stmt;
}

JS_EXPR *init_expression() {
  JS_EXPR *expr = (JS_EXPR*)malloc(sizeof(JS_EXPR));
  expr->type = IND_EXPR;
  expr->children = NULL;

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
