// for `getopt`
#include <unistd.h>

#include "lex.h"

int main(int narg, char **args) {
  TOKEN_STREAM *ts;

  if (init_token_stream(&ts)) {
    printf("ITS A TRAP");
    exit(1);
  }

  do {
    put_token(&ts->data, false);
  } while (!next_token(&ts, 1));
}
