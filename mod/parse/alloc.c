#include "parse.h"

JS_STMT *init_statement() {
  JS_STMT *stmt = (JS_STMT*)malloc(sizeof(JS_STMT));
  stmt->type = IND_STMT;
  stmt->children = NULL;

  return stmt;
}

JS_EXPR *init_expression() {
  JS_EXPR *expr = (JS_EXPR*)malloc(sizeof(JS_EXPR));
  expr->type = IND_EXPR;
  expr->children = NULL;
  expr->statements = NULL;

  return expr;
}

void destroy_statement(JS_STMT *stmt) {
  free(stmt);
}

void destroy_expression(JS_EXPR *expr) {
  free(expr);
}
