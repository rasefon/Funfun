%{
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "ff.h"
#define YYDEBUG 1

void yyerror(char *s, ...);
extern int yylineno;
%}

%union {
  char  *str_val;
  struct FfNode *node;
  struct Stmts *stmt;
}

%token QUOTE ATOM EQ CAR CDR CONS COND
%token <str_val>  ID

%type <node> list op args
%type <stmt> stmts

%start progm

%%

progm
  : stmts {
    Interpreter *inter = ff_get_interpreter();
    inter->progm = $1;
  }

stmts
  : list {
    $$ = ff_create_stmts($1);
  }
  | stmts list{
    $$ = ff_link_stmts($1, $2);
  }
;

list
  : '(' op args ')' {
    $$ = ff_link_child($2, $3);
  }
  |
  '(' ')' {
  }
;

args
  : ID {
    $$ = ff_create_arg_node($1);
  }
  | list 
  | args ID {
    FfNode *right_node = ff_create_arg_node($2);
    $$ = ff_link_sibling($1, right_node);
    dbg_info("%s ", right_node->nd_arg_val);
  }
  | args list {
    $$ = ff_link_sibling($1, $2);
  }
;

op
  : QUOTE {
    $$ = ff_create_op_node(op_quote);
    dbg_info("quote ");
  }
  | ATOM {
    $$ = ff_create_op_node(op_atom);
    dbg_info("atom ");
  }
  | EQ {
    $$ = ff_create_op_node(op_eq);
    dbg_info("eq ");
  }
  | CAR {
    $$ = ff_create_op_node(op_car);
    dbg_info("car ");
  } 
  | CDR {
    $$ = ff_create_op_node(op_cdr);
    dbg_info("cdr ");
  }
  | CONS  {
    $$ = ff_create_op_node(op_cons);
    dbg_info("cons ");
  }
  | COND  {
    $$ = ff_create_op_node(op_cond);
    dbg_info("cond ");
  }
;

%%

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stdout, "%d: error: ", yylineno);
  vfprintf(stdout, s, ap);
  fprintf(stdout, "\n");
}

