#ifndef PARSE_H
#define PARSE_H

// NOTE: It seems to be typical to consider a block
// as a type of statment rather than as an expression.
// This makes sense because a block doesn't produce a
// value, but in my model it is easier to simply consider
// it as a type of expression that is neither an LVALUE
// nor an RVALUE, because my expressions have a list of
// things, and some of them have to be expressions and
// some of them have to be blocks, so I may as well unify
// those.

typedef enum {
  ASSIGN,       // 2 exprs linked by  ='s (always LVALUE then RVALUE)
  ASSIGN_PLUS,  // 2 exprs linked by *='s (always LVALUE then RVALUE)
  ASSIGN_TIMES, // 2 exprs linked by +='s (always LVALUE then RVALUE)
  IF,           // 1 expr  followed by one or two blocks
  FOR,          // for (three child expressions followed by a block)
  FOR_IN,       // for (a in b) (two child expressions and then a block)
  FUNCTION,     // function(a,...,z) (any number of child expressions ending with a block)
  WHILE,        // while (...) (one expressions and then a block)
  DO_WHILE,     // do {...} while (...) (one block followed by an expression)
  SWITCH,       // ??
  WITH,         // one expression then a block
  TRY_CATCH     // block, possible expression, then another block
} statement_type;

typedef enum {
  LIT_NUM,
  LIT_STR,
  CALL,      // children are arbitrary expression followed by a list of arguments
  SYMBOL,    // string data
  DECL,      // string data (similar to SYMBOL but declares a )
  BLOCK,     // arbitrary list of statements
  ARRAY,     // arbitrary list of expressions
  OBJECT,    // alternates between symbols and RVALUE types
  PLUS,      // two arbitrary expressions
  POST_INCR, // one arbitrary expression
  PRE_INCR,  // one arbitrary expression
  TIMES,     // two arbitrary expressions
  EQUALS,    // two arbitrary expressions
  LT,        // two arbitrary expressions
  GT,        // two arbitrary expressions
  LET,       // two arbitrary expressions
  GET        // two arbitrary expressions
} expression_type;

extern const char *stmt_type_names[8];
extern const char *expr_type_names[8];

typedef struct JS_EXPRS JS_EXPRS;
typedef struct JS_STMTS JS_STMTS;

typedef struct {
  statement_type type;

  JS_EXPRS *children;
} JS_STMT;

typedef struct {
  // TODO: needs space to stores literal data
  //  including string (for string literals and symbols)
  //  integer
  //  floating point
  //  etc

  expression_type type;

  JS_EXPRS *children;
  JS_STMTS *statements;
} JS_EXPR;

struct JS_EXPRS {
  JS_EXPR data;
  JS_EXPRS *next;
};

struct JS_STMTS {
  JS_STMT data;
  JS_STMTS *next;
};

// LVALUE and RVALUE checking macros

// something like:
// int read_statement(TOKEN_STREAM *);
// int read_expression(TOKEN_STREAM *);

#endif
