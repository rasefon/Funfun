#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

extern FILE *yyin;
extern int yyparse();
static unsigned int s_indent = 4;

void print_list(FfNode* root_nd)
{
  assert(root_nd);

  FfNode* curr_nd = root_nd;
  if (curr_nd->nd_type == nd_op) {
    printf("Op: %d\n", curr_nd->nd_op_val);
    if (curr_nd->child) {
      printf("%*s", s_indent, "");
      s_indent << 1;
      print_list(curr_nd->child);
      s_indent >> 1;
    }
  }
  else {
    printf("%s ", curr_nd->nd_arg_val);
    curr_nd = curr_nd->next;
    while (curr_nd) {
      print_list(curr_nd);
      curr_nd = curr_nd->next; 
    }
  }
}

void print_tree(Stmts *root_stmt)
{
  assert(root_stmt != NULL);
  int indent = 0; 
  Stmts *curr_stmt = root_stmt;
  while(curr_stmt) {
    print_list(curr_stmt->list);
    printf("\n\n");
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

  fcloseall();

  //test
  print_tree(g_inter->progm);

  free(g_inter);

  /*printf("%*s%s\n", 4, "", "aaaa"); */

  return 0;
}
