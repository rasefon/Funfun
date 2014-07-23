#include <assert.h>
#include <stdlib.h>
#include "ff.h"

static Interpreter *s_interpreter;

Interpreter* ff_get_interpreter()
{
  return s_interpreter;
}

void ff_set_interpreter(Interpreter *interpreter)
{
  s_interpreter = interpreter;
}

Stmts* ff_create_stmts(FfNode *list)
{
  assert(list != NULL);
  Stmts *s = malloc(sizeof(Stmts));
  s->list = list;
  return s;
}

Stmts* ff_link_stmts(Stmts *head, FfNode *nd)
{
  assert(head != NULL && nd != NULL);
  Stmts *new_s = malloc(sizeof(Stmts));
  Stmts *s = head;
  while(s->next) {
    s = s->next;
  }
  s->next = new_s;
  return s;
}
