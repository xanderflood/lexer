#include "local.h"
#include "string.h"

// return 0 if the token was consumed
int update_state(JS_EXPR *current, TOKEN *tok) {
  assert (EXPR_TYPE_INTERNAL_NODE(current));

  // if the new token is NULL, then this function
  // is being called because `current` has a new child

  // otherwise, current's primary child became terminal
  // in a previous step, and this is being called because
  // of a new token, such as a comma or close paren

  // TOOD: most of these asserts should be
  //   if/elses with syntax error return codes
  EXPR_FORMAT ef = type_info[current->type].format;
  if (ef == INV_EXPR_FMT)
    assert(0);

  else if (ef == LEAF_EXPR_FMT)
    assert(0);

  else if (ef == ROOT_EXPR_FMT) {
    if (tok == NULL) {
      if (current->state == NTERM_CS_EXST)
        current->state = NTERM_SV_EXST;
      else
        return -1; // raise(EXPECTED_EXPRESSION);
    } else {
      if (! strcmp(tok->s, ";")) {
        if (EXPECTING_SIGNAL_STATE(current))
          current->state = FINISHED_EXST;
        else
          return -1; // raise(UNEXPECTED_TOKEN);
      } else
        return -1; // raise(EXPECTED_SEMICOLON);
      }
  }

  else if (ef == VAR_DECL_EXPR_FMT) {
    if (current->state == NTERM_C_EXST) {
      if (tok)
        return -1; // raise UNEXPECTED_TOKEN
      else
        current->state = FINISHED_VIS_EXST;
    } else {
      assert(0);
    }
  }

  else if (ef == ARRAY_EXPR_FMT) {
    if (current->state == NTERM_C_EXST) {
      if (tok)
        return -1; //raise UNEXPECTED_TOKEN
      else
        current->state = NTERM_SV_EXST;
    } else if (current->state == NTERM_SV_EXST) {
      if (! strcmp(tok->s, ","))
        current->state = NTERM_C_EXST;
      else if (! strcmp(tok->s, "]"))
        current->state = FINISHED_EXST;
      else
        return -1; // raise EXPECTED_COMMA_OR_RSQBRACK
    } else
      assert(0);
  }

  else if (ef == OBJECT_EXPR_FMT) {
  }

  else if (ef == FCN_DEFN_EXPR_FMT) {
  }

  else if (ef == PREFIX_UNOP_EXPR_FMT) {
    if(current->state == NTERM_C_EXST) {
      if (tok)
        return -1; //raise UNEXPECTED_TOKEN
      else
        current->state = FINISHED_VIS_EXST;
    }
  }

  else if (ef == POSTFIX_UNOP_EXPR_FMT) {
    if(current->state == NTERM_C_EXST) {
      if (tok)
        return -1; //raise UNEXPECTED_TOKEN
      else
        current->state = FINISHED_EXST;
    }
  }

  else if (ef == INFIX_BINOP_EXPR_FMT) {
    if(current->state == NTERM_C_EXST && current->child_count == 0) {
      if (tok)
        return -1; //raise UNEXPECTED_TOKEN
    } else if(current->state == NTERM_C_EXST && current->child_count == 1) {
      if (tok)
        return -1; //raise UNEXPECTED_TOKEN
      else
        current->state = FINISHED_VIS_EXST;
    }
  }

  else if (ef == PAREN_EXPR_FMT) {
    if (current->state == NTERM_C_EXST) {
      if (tok)
        return -1;
      else
        current->state = NTERM_SV_EXST;
    } else if (current->state == NTERM_SV_EXST) {
      if (! strcmp(tok->s, ")"))
        current->state = FINISHED_EXST;
      else
        return -1;// raise EXPECTED_RPAREN
    } else
      assert(0);
  }

  else if (ef == COMP_ACCESS_EXPR_FMT) {
    if (current->state == NTERM_C_EXST && current->child_count == 0) {
      if (tok)
        return -1;
      else
        current->state = NTERM_C_EXST;
    } else if (current->state == NTERM_C_EXST && current->child_count == 1) {
      if (tok)
        return -1;
      else
        current->state = NTERM_SV_EXST;
    } else if (current->state == NTERM_SV_EXST) {
      if (! strcmp(tok->s, "]"))
        current->state = FINISHED_EXST;
      else
        assert(0);
    } else
      assert(0);
  }

  else if (ef == NEW_ARGS_EXPR_FMT) {
  }

  else if (ef == FCN_CALL_EXPR_FMT) {
  }

  else if (ef == CONF_TRIOP_EXPR_FMT) {
    if (current->state == NTERM_C_EXST && current->child_count == 0) {
      if (tok)
        return -1;
      else
        current->state = NTERM_C_EXST;
    } else if (current->state == NTERM_C_EXST && current->child_count == 1) {
      if (tok)
        return -1;
      else
        current->state = NTERM_SV_EXST;
    } else if (current->state == NTERM_SV_EXST) {
      if (! strcmp(tok->s, ":"))
        current->state = NTERM_C_EXST;
      else
        return -1; // raise EXPECTED_COLON
    } else if (current->state == NTERM_C_EXST && current->child_count == 2) {
      if (tok)
        return -1;
      else
        current->state = FINISHED_VIS_EXST;
    } else
      assert(0);
  }

  // TODO: implement

  return 0;
}

// -2 = initially terminal AND expectant
// -1 = initially terminal and NOT expectant (i.e. FINISHED)
//  0 = invalid
//  1 = waiting on signal tokens before expecting children
//  2 = initally expectant and non-terminal

const char expr_type_initial_states[NUM_EXPR_FORMATS] = {
  INVALID_EXST,  // INV_EXPR_FMT,

  FINISHED_EXST, // LEAF_EXPR_FMT

  NTERM_CS_EXST, // ROOT_EXPR_FMT

  NTERM_C_EXST,  // VAR_DECL_EXPR_FMT
  NTERM_CS_EXST, // ARRAY_EXPR_FMT
  NTERM_CS_EXST, // OBJECT_EXPR_FMT
  NTERM_C_EXST,  // FCN_DEFN_EXPR_FMT

  NTERM_C_EXST,  // PREFIX_UNOP_EXPR_FMT
  NTERM_C_EXST,  // POSTFIX_UNOP_EXPR_FMT
  NTERM_C_EXST,  // INFIX_BINOP_EXPR_FMT

  NTERM_C_EXST,  // PAREN_EXPR_FMT
  NTERM_C_EXST,  // COMP_ACCESS_EXPR_FMT
  NTERM_C_EXST,  // NEW_ARGS_EXPR_FMT
  NTERM_C_EXST,  // FCN_CALL_EXPR_FMT

  NTERM_C_EXST   // CONF_TRIOP_EXPR_FMT
};

// does it start expecting a child or no?
void init_state(JS_EXPR *expr) {
  expr->state = expr_type_initial_states[expr->type];
}

EXPR_TYPE interpret_token_as_verb(TOKEN *tok) {
  if (tok->type == NUM_TOK)
    return LIT_NUM_EXPR;     // numeric literal
  else if (tok->type == LIT_TOK)
    return LIT_STR_EXPR;     // string literal
  else if (tok->type == SYM_TOK)
    return SYMBOL_EXPR;      // string data
  else if (tok->type == OPR_TOK && strcmp(tok->s, "var"))
    return DECL_EXPR;        // string data (similar to SYMBOL but declares a )
  else if (tok->type == PNC_TOK && strcmp(tok->s, "["))
    return ARRAY_EXPR;       // arbitrary list of expressions
  else if (tok->type == PNC_TOK && strcmp(tok->s, "{"))
    return OBJECT_EXPR;      // alternates between symbols and RVALUE types
  else if (tok->type == OPR_TOK && strcmp(tok->s, "function"))
    return FUNCTION_EXPR;    // a function definition
  else if (tok->type == PNC_TOK && strcmp(tok->s, "("))
    return PAREN_EXPR;       // (a)
  else if (tok->type == OPR_TOK && strcmp(tok->s, "new"))
    return NEW_ARGS_EXPR;    // new a (b, ...)
                             // TODO: new a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!"))
    return NOT_EXPR;         // ! a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "~"))
    return BITNOT_EXPR;      // ~ a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+"))
    return POS_EXPR;         // + a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "-"))
    return NEG_EXPR;         // - a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "++"))
    return PRE_INCR_EXPR;    // ++ a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "--"))
    return PRE_DECR_EXPR;    // -- a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "typdeof"))
    return TYPEOF_EXPR;      // typdeof a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "void"))
    return VOID_EXPR;        // void a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "delete"))
    return DELETE_EXPR;      // delete a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "yield"))
    return YIELD_EXPR;       // yield a
  else if (tok->type == OPR_TOK && strcmp(tok->s, "yield*"))
    return YIELD_STAR_EXPR;  // yield *a
  else if (tok->type == PNC_TOK && strcmp(tok->s, "..."))
    return SPREAD_EXPR;      // ... a
  else
    return IND_EXPR;
}

EXPR_TYPE interpret_token_as_noun(TOKEN *tok) {
  assert(tok->type == OPR_TOK || tok->type == PNC_TOK);

  if (tok->type == OPR_TOK && strcmp(tok->s, "."))
    return ACCESS_EXPR;          // a.b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "["))
    return COMP_ACCESS_EXPR;      // a[b]
  else if (tok->type == OPR_TOK && strcmp(tok->s, "("))
    return CALL_EXPR;             // a (...)
  else if (tok->type == OPR_TOK && strcmp(tok->s, "++"))
    return POST_INCR_EXPR;        // a ++
  else if (tok->type == OPR_TOK && strcmp(tok->s, "--"))
    return POST_DECR_EXPR;        // a --
  else if (tok->type == OPR_TOK && strcmp(tok->s, "**"))
    return EXP_EXPR;              // a ** b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "*"))
    return TIMES_EXPR;            // a * b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "/"))
    return QUOT_EXPR;             // a / b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "%"))
    return MOD_EXPR;              // a % b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+"))
    return PLUS_EXPR;             // a + b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "-"))
    return MINUS_EXPR;            // a - b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<<"))
    return BSLEFT_EXPR;           // a << b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>"))
    return BSRIGHT_EXPR;          // a >> b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>>"))
    return UBSRIGHT_EXPR;         // a >>> b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<"))
    return LT_EXPR;               // a < b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">"))
    return GT_EXPR;               // a > b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<="))
    return LET_EXPR;              // a <= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">="))
    return GET_EXPR;              // a >= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "in"))
    return IN_EXPR;               // a in b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "instanceof"))
    return INSTANCEOF_EXPR;       // a instanceof b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "=="))
    return EQ_EXPR;               // a == b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!="))
    return INEQ_EXPR;             // a != b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "==="))
    return STRICT_EQ_EXPR;        // a === b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "!=="))
    return STRICT_INEQ_EXPR;      // a !== b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&"))
    return BAND_EXPR;             // a & b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "^"))
    return BXOR_EXPR;             // a ^ b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "|"))
    return BOR_EXPR;              // a | b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&&"))
    return LAND_EXPR;             // a && b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "||"))
    return LOR_EXPR;              // a || b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "?"))
    return COND_EXPR;             // a ? b : c
  else if (tok->type == OPR_TOK && strcmp(tok->s, "="))
    return ASSIGN_EXPR;           // a = b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "+="))
    return ASSIGN_PLUS_EXPR;      // a += b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "*="))
    return ASSIGN_TIMES_EXPR;     // a *= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "**="))
    return ASSIGN_EXP_EXPR;       // a **= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "/="))
    return ASSIGN_QUOT_EXPR;      // a /= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "%="))
    return ASSIGN_MOD_EXPR;       // a %= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "<<="))
    return ASSIGN_BSLEFT_EXPR;    // a <<= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>="))
    return ASSIGN_BSRIGHT_EXPR;   // a >>= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, ">>>="))
    return ASSIGN_UBSRIGHT_EXPR;  // a >>>= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "&="))
    return ASSIGN_BAND_EXPR;      // a &= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "^="))
    return ASSIGN_BXOR_EXPR;      // a ^= b
  else if (tok->type == OPR_TOK && strcmp(tok->s, "|="))
    return ASSIGN_BOR_EXPR;       // a |= b
  else if (tok->type == PNC_TOK && strcmp(tok->s, ","))
    return COMMA_EXPR;            // a, b (valuates to b)
  else
    return IND_EXPR;
}
