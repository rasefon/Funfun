#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

extern FILE *yyin;
extern int yyparse();
static unsigned int s_indent = 4;

char *g_op_type_arr[8] = {"", "quote", "atom", "eq", "car", "cdr", "cons", "cond"};
FfNode* eval_list(FfNode *root);

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
      /*printf(" op: %s", g_op_type_arr[curr_nd->node_val_op]);*/
      printf("%s ", g_op_type_arr[curr_nd->node_val_op]);
    }
    else {
      /*printf(" id: %s", curr_nd->node_val_id);*/
      printf("%s ", curr_nd->node_val_id);
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

void print_eval_results(Stmts *root_stmt)
{
  assert(!!root_stmt);
  static int s_dbg_count = 0;

  Stmts *curr_stmt = root_stmt;
  while(curr_stmt) {
    printf("=================================================\n");
    FfNode *result = eval_list(curr_stmt->list);
    print_list(result);
    printf("\n\neval count:%d\n", s_dbg_count++);
    printf("\n");
    curr_stmt = curr_stmt->next;
  }
}

FfNode* eval_list(FfNode *root)
{
  FfNode *result = NULL;
  //currently just print out the evaluated result.
  FfNode *curr_nd = root;
  if (curr_nd->type == node_op) {
    FfNode *next_nd = curr_nd->next;
    switch (curr_nd->node_val_op) {
      case op_quote:
        {
          if (next_nd != NULL) {
            if (next_nd->type == node_op) {
              fprintf(stderr, "quote shouldn't use operator as parameter.\n");
              exit(-1);
            }
            else if (next_nd->type == node_id) {
              if (!!next_nd->next) {
                fprintf(stderr, "quote can't use more than 1 parameters, when the first parameter is an id.\n");
                exit(-1);
              }
              else {
                result = ff_create_id_node(next_nd->node_val_id);
                return result;
              }
            }
            else {
              if (next_nd->is_empty) {
                result = ff_create_empty_node();
              }
              else {

                FfNode *sub_node = next_nd->node_val_list;
                result = ff_copy_node(sub_node);
                while (sub_node->next) {
                  sub_node = sub_node->next;
                  FfNode *tmp_node = ff_copy_node(sub_node);
                  ff_link_node(result, tmp_node);
                }
              }

              return result;
            }
          }
          else {
            fprintf(stderr, "quote should have at least one arg.\n");
            exit(-1);
          }
          break;
        }
      case op_atom:
        {
          if (next_nd) {
            if (next_nd->type == node_list) {
              FfNode *tmp = eval_list(next_nd);
              assert(!!tmp);

              result = ff_create_empty_node();
              if(tmp->is_empty || tmp->type != node_list) {
                result->is_empty = true;
              }

              return result;
            }
            else {
              fprintf(stderr, "quote should use list as parameter.\n");
              exit(-1);
            }
          }
          else {
            fprintf(stderr, "atom should have at least one arg.\n");
            exit(-1);
          }
          break;
        }
      case op_eq:
        {
          break;
        }
      case op_car:
        {
          break;
        }
      case op_cdr:
        {
          break;
        }
      case op_cons:
        {
          break;
        }
      case op_cond:
        {
          break;
        }
      default:
        {
        }
    }
  }
  else if (curr_nd->type == node_list) {
    return eval_list(curr_nd->node_val_list);
  }
  else {
    fprintf(stderr, "First elem should be an operator.\n");
    exit (-1);
  }
  //error
  return result;
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
  /*print_tree(g_inter->progm);*/
  print_eval_results(g_inter->progm);

  free(g_inter);

  fcloseall();
  return 0;
}
