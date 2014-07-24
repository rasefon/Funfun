#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ff.h"

extern FILE *yyin;
extern int yyparse();
static unsigned int s_indent = 4;
static bool s_dt = false;

char *g_op_type_arr[8] = {"", "quote", "atom", "eq", "car", "cdr", "cons", "cond"};
FfNode* eval_list(FfNode *root);

void print_list(FfNode *root_nd)
{
  assert(!!root_nd);

  FfNode* curr_nd = root_nd;

  if (curr_nd->is_empty && curr_nd->type != node_bool) {
    printf("nil ");
    return;
  }

  if (curr_nd->type != node_list) {
    if (curr_nd->type == node_op) {
      if (s_dt) {
        printf("op:%s\t", g_op_type_arr[curr_nd->node_val_op]);
      }
      else {
        printf("%s ", g_op_type_arr[curr_nd->node_val_op]);
      }
    }
    else if(curr_nd->type == node_id) {
      if (s_dt) {
        printf("id:%s\t", curr_nd->node_val_id);
      }
      else {
        printf("%s ", curr_nd->node_val_id);
      }
    }
    else {
      // bool type
      printf("%s ", curr_nd->node_val_bool ? "T" : "F");
    }

    if (curr_nd->next) {
      print_list(curr_nd->next);
    }
  }
  else {
    if (curr_nd->node_val_list) {
      if (s_dt) {
        printf("\n---");
        printf("%*s", s_indent, "");
        s_indent *= 2;
      }
      printf("( ");
      print_list(curr_nd->node_val_list);
      if (s_dt) {
        s_indent /= 2;
      }

      if (curr_nd->next) {
        print_list(curr_nd->next);
      }
      printf(")");
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
          if (next_nd) {
            if (next_nd->type == node_op) { //op
              fprintf(stderr, "quote shouldn't use operator as parameter.\n");
              exit(-1);
            }
            else if (next_nd->type == node_id) { //id
              if (!!next_nd->next) {
                fprintf(stderr, "quote can't use more than 1 parameters, when the first parameter is an id.\n");
                exit(-1);
              }
              else {
                result = ff_create_id_node(next_nd->node_val_id);
                return result;
              }
            }
            else { //list
              if (next_nd->is_empty) {
                result = ff_create_empty_node();
              }
              else {
               result = ff_create_list_node(next_nd->node_val_list);

                /*FfNode *sub_node = ff_copy_node(next_nd->node_val_list);*/
                /*result = ff_create_list_node(sub_node);*/
                /*while (sub_node->next) {*/
                  /*sub_node = sub_node->next;*/
                  /*FfNode *tmp_node = eval_list(sub_node);*/
                  /*ff_link_node(result, tmp_node);*/
                /*}*/
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

              if(tmp->is_empty || tmp->type != node_list) {
                result = ff_create_bool_node(true);
              }
              else {
                result = ff_create_bool_node(false);
              }

              return result;
            }
            else {
              fprintf(stderr, "atom should use list as parameter.\n");
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
          if (next_nd && next_nd->next && !next_nd->next->next) {
            FfNode* first_nd = eval_list(next_nd);
            assert(!!first_nd);
            FfNode* second_nd = eval_list(next_nd->next);
            assert(!!second_nd);

            if(first_nd->is_empty && second_nd->is_empty) {
              return ff_create_bool_node(true);
            }

            if (first_nd->type == second_nd->type) {
              if (first_nd->type == node_id) {
                if (0 == strcmp(first_nd->node_val_id, second_nd->node_val_id)) {
                  return ff_create_bool_node(true);
                }
              }
            }

            if (second_nd->node_val_bool == first_nd->node_val_bool) {
              return ff_create_bool_node(true);
            }

            return ff_create_bool_node(false);
          }
          else {
            fprintf(stderr, "eq should have 2 args.\n");
            exit(-1);
          }
          break;
        }
      case op_car:
      case op_cdr:
        {
          if (next_nd && !next_nd->next) {
            if (next_nd->type == node_list) {
              if (next_nd->is_empty) {
                return ff_create_empty_node();
              }
              else {
                FfNode *tmp = eval_list(next_nd);
                if (tmp->is_empty) {
                  return ff_create_empty_node();
                }

                if(tmp->type == node_list) {
                  if (curr_nd->node_val_op == op_car) {
                    if (tmp->node_val_list->type == node_list) {
                      return tmp->node_val_list;
                    }
                    else {
                      return ff_create_id_node(tmp->node_val_list->node_val_id);
                    }
                  }
                  else {
                    FfNode *head = tmp->node_val_list;
                    head = head->next;
                    if (head) {
                      return ff_create_list_node(head);
                    }
                    else {
                      return ff_create_empty_node();
                    }
                  }
                }
                else {
                  if (curr_nd->node_val_op == op_car) {
                    return ff_create_id_node(tmp->node_val_id);
                  }
                  else {
                    return ff_create_empty_node();
                  }
                }
              }
            }
            else {
              fprintf(stderr, "car arg should be a list.\n");
              exit(-1);
            }
          }
          else {
            fprintf(stderr, "car should only have 1 arg.\n");
            exit(-1);
          }
          break;
        }
      case op_cons:
        {
          if (next_nd && next_nd->next && !next_nd->next->next) {
            FfNode* first_nd = eval_list(next_nd);
            assert(!!first_nd);
            FfNode* second_nd = eval_list(next_nd->next);
            assert(!!second_nd);

            if (first_nd->type == node_id && second_nd->type == node_list) {
              first_nd->next = second_nd->node_val_list;
              second_nd->node_val_list = first_nd;
              return second_nd;
            }
            else {
              fprintf(stderr, "cons error.\n");
              exit(-1);
            }
          }
          else {
            fprintf(stderr, "cons should have 2 args.\n");
            exit(-1);
          }
          break;
        }
      case op_cond:
        {
          while(next_nd) {
            if (next_nd->type == node_list) {
              FfNode *cond_list = next_nd->node_val_list;
              assert(cond_list);
              FfNode *expr_list = cond_list->next;
              assert(expr_list);
              if(cond_list->type == node_list && expr_list->type == node_list) {
                FfNode *cond_ret = eval_list(cond_list);
                assert(cond_ret);

                if (cond_ret->type == node_bool) {
                  if (cond_ret->node_val_bool) {
                    return eval_list(expr_list);
                  }
                  else {
                    next_nd = next_nd->next;
                  }
                }
                else {
                  fprintf(stderr, "cond condition list error.\n");
                  exit(-1);
                }
              }
              else {
                fprintf(stderr, "cond error.\n");
                exit(-1);
              }
            }
            else {
              fprintf(stderr, "cond arg should be a list.\n");
              exit(-1);
            }
          }

          return ff_create_empty_node();
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
  /*extern int yydebug;*/
  /*yydebug = 1;*/

  if (argc != 2 && argc != 3) {
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

  if (argc == 3 && 0 == strcmp(argv[2], "/dt")) {
    s_dt = true;
    print_tree(g_inter->progm);
  }
  else {
    print_eval_results(g_inter->progm);
  }

  free(g_inter);

  fclose(yyin);
  return 0;
}
