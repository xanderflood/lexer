#include "lex.h"

void put_token(TOKEN *cur, bool text_mode) {
  if (text_mode)
    printf("%s %s\n", token_type_names[cur->type], cur->s);
  else
    printf("%c%s\n", cur->type, cur->s);
}

// returns 0 when successful
int get_token(TOKEN *cur) {
  return scanf("%c%s\n", (char*)&(cur->type), cur->s) < 0;
}
