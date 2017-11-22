#include "local.h"

static void print_expression_contents(JS_EXPR *expr, int i, int parent_id);
static void print_expression_tree_helper(JS_EXPR *expr, int *i);

void print_expression_tree(JS_EXPR *expr) {
  int i = 0;

  print_expression_contents(expr, 0, 0);
  print_expression_tree_helper(expr, &i);
}

static void print_expression_tree_helper(JS_EXPR *expr, int *i) {
  EXPR_ITEM *ei = expr->children;
  int id  = *i;

  if (ei == NULL) return;

  do {
    print_expression_contents(expr, (*i)++, id);
    print_expression_tree_helper(ei->data, i);
  } while ((ei = ei->next) != NULL);
}

static void print_expression_contents(JS_EXPR *expr, int id, int parent_id) {
  printf("%u\tparent: %u\t%s\n", id, parent_id, type_info[expr->type].name);
}
