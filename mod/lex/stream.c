#include "lex.h"

// writing tokens
void put_token(TOKEN *cur, bool text_mode) {
  if (text_mode)
    printf("%s\t%s\n", token_type_names[cur->type], cur->s);
  else
    printf("%c%s\n", cur->type, cur->s);
}

// reading tokens (TOKEN_STREAM)
static int load_token(TOKEN *token) {
  return scanf("%c%s\n", (char*)(&token->type), token->s) < 0;
}

int init_token_stream(TOKEN_STREAM **ts) {
  *ts = (TOKEN_STREAM *)malloc(sizeof(TOKEN_STREAM));

  (*ts)->next = NULL;
  return load_token(&(*ts)->data);
}

int next_token(TOKEN_STREAM **ts, char destroy) {
  TOKEN_STREAM *ts_old;
  int ret;

  if ((*ts)->next == NULL) {
    (*ts)->next = (struct TOKEN_STREAM*)malloc(sizeof(TOKEN_STREAM));
    (*ts)->next->next = NULL;

    ret = load_token(&(*ts)->next->data);
    if (ret) return ret;
  }

  if (destroy) {
    ts_old = *ts;
    *ts = (*ts)->next;
    free(ts_old);
  }

  return 0;
}

int peek_token(TOKEN_STREAM *ts, uint k, TOKEN **tok) {
  TOKEN_STREAM *to;
  int ret = seek_token(ts, k, &to);
  if (ret) return ret;

  *tok = &to->data;
  return 0;
}

int seek_token(TOKEN_STREAM *ts, uint k, TOKEN_STREAM **to) {
  int ret;

  // scan through what's already loaded
  while (k > 0 && ts->next) {
    ts = ts->next;
    k--;
  }

  while (k > 0) {
    ret = next_token(&ts->next, 0);
    if (ret) return ret;
    k--;
  }

  *to = ts;
  return 0;
}

// returns 0 when successful
int get_token(TOKEN *cur) {
  return scanf("%c%s\n", (char*)&(cur->type), cur->s) < 0;
}
