#ifndef FF_H
#define FF_H
#include "ast.h"

typedef struct Stmts {
  FfNode *list;
  struct Stmts *next;
} Stmts;

typedef struct Interpreter {
  Stmts *progm;
} Interpreter;

Interpreter* ff_get_interpreter();
void ff_set_interpreter(Interpreter *interpreter);
Stmts* ff_create_stmts(FfNode *list);
Stmts* ff_link_stmts(Stmts *head, FfNode *nd);

#endif
