#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

extern FILE *yyin;
extern int yyparse();
static unsigned int s_indent = 4;

static char *op_type_arr[8] = {"", "quote", "atom", "eq", "car", "cdr", "cons", "cond"};

void print_list(FfNode *root_nd)
{
  assert(!!root_nd);

  FfNode* curr_nd = root_nd;

  if (curr_nd->is_empty) {
    printf(" nil ");
    return;
  }

  if (curr_nd->type != node_list) {
    if (curr_nd->type == node_op) {
      printf(" op: %s", op_type_arr[curr_nd->node_val_op]);
    }
    else {
      printf(" id: %s", curr_nd->node_val_id);
    }

    if (curr_nd->next) {
      print_list(curr_nd->next);
    }
  }
  else {
    if (curr_nd->node_val_list) {
      printf("\n");
      printf("%*s", s_indent, "");
      s_indent *= 2;
      print_list(curr_nd->node_val_list);
      s_indent /= 2;

      if (curr_nd->next) {
        print_list(curr_nd->next);
      }
    }
  }
}

void print_tree(Stmts *root_stmt)
{
  assert(!!root_stmt );
  static int s_dbg_count = 0;

  Stmts *curr_stmt = root_stmt;
  while(curr_stmt) {
    printf("=================================================\n");
    print_list(curr_stmt->list);
    printf("\n\nstmt count:%d\n", s_dbg_count++);
    printf("\n");
    curr_stmt = curr_stmt->next;
  }
}

int main(int argc, char **argv)
{
  extern int yydebug;
  /*yydebug = 1;*/

  if (argc != 2) {
    fprintf(stderr, "Usage: \n");
    fprintf(stderr, "arg1: lisp file name.\n");
    return -1;
  }

  yyin = fopen(argv[1], "r");
  if (!yyin) {
    perror(argv[1]);
    return -1;
  }

  //Create an interpreter.
  Interpreter *g_inter = malloc(sizeof(Interpreter));
  ff_set_interpreter(g_inter);

  if (yyparse()) {
    // error happened.
    fprintf(stderr, "Failed to parse file %s\n", argv[1]);
  }
  else {
    fprintf(stdout, "Succeeded to parse file %s\n", argv[1]);
  }

  //test
  /*printf("%p, %p\n", g_inter, g_inter->progm);*/
  print_tree(g_inter->progm);

  free(g_inter);

  fcloseall();
  return 0;
}
