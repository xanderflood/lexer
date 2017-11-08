// for `getopt`
#include <unistd.h>

#include "lex.h"
#include "parse.h"

int main(int narg, char **args) {
  TOKEN_STREAM *ts;

  if (init_token_stream(&ts)) {
    printf("ITS A TRAP");
    exit(1);
  }

  JS_STMT *stmt = init_statement();
  if (! read_statement(stmt)) {
  	printf("FAILED");
  	return 1;
  }

  return 0;
}
