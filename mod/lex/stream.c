#include "lex.h"

// writing tokens
void put_token(TOKEN *cur, bool text_mode) {
  if (text_mode)
    printf("%s %s\n", token_type_names[cur->type], cur->s);
  else
    printf("%c%s\n", cur->type, cur->s);
}

// reading tokens (TOKEN_STREAM)
static int load_token(TOKEN_STREAM *ts) {
  assert(ts->next == NULL);

  ts->next = (struct TOKEN_STREAM*)malloc(sizeof(TOKEN_STREAM));
  ts->next->next = NULL;
  return scanf("%c%s\n", (char*)&(ts->next->data.type), ts->next->data.s) < 0;
}

int init_token_stream(TOKEN_STREAM **ts) {
  *ts = (TOKEN_STREAM *)malloc(sizeof(TOKEN_STREAM));

  (*ts)->next = NULL;
  return scanf("%c%s\n", (char*)&((*ts)->data.type), (*ts)->data.s) < 0;
}

int next_token(TOKEN_STREAM **ts) {
  TOKEN_STREAM *ts_old;
  int ret;

  if ((*ts)->next == NULL) {
    ret = load_token(*ts);
    if (ret) return ret;
  }

  ts_old = *ts;
  *ts = (*ts)->next;
  free(ts_old);

  return 0;
}

int peek_token(TOKEN_STREAM *ts, TOKEN **t, uint k) {
  int ret;

  while (k > 0) {
    if (!ts->next) {
      ret = load_token(ts);
      if (ret) return ret;
    }

    ts = ts->next;
  }

  *t = &ts->data;

  return 0;
}

// returns 0 when successful
int get_token(TOKEN *cur) {
  return scanf("%c%s\n", (char*)&(cur->type), cur->s) < 0;
}
