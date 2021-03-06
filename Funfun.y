%{
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "ff.h"
#define YYDEBUG 1

void yyerror(char *s, ...);
extern int yylineno;
static int s_dbg_count = 0;
%}

%union {
  char  *str_val;
  struct FfNode* node;
  struct Stmts *stmt;
}

%token QUOTE ATOM EQ CAR CDR CONS COND
%token <str_val>  ID

%type <node> op list elems
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
    dbg_info("\nstmt created.\n");
    $$ = ff_create_stmts($1);
  }
  | stmts list{
    dbg_info("\nstmt linked.\n\n");
    $$ = ff_link_stmts($1, $2); 
  }
;

list
  :
   '(' elems ')' {
    $$ = ff_create_list_node($2);
  }
  |
  '(' ')' {
    $$ = ff_create_empty_node();
  }
;

elems
  : op {
    dbg_info("\t[op:%s]~%d\n", $1, s_dbg_count++);
  }
  | ID {
    dbg_info("\t[id:%s]~%d\n", $1, s_dbg_count++);
    $$ = ff_create_id_node($1); 
  }
  | list {
    dbg_info("\t[list]~%d\n", s_dbg_count++);
  }
  | elems ID {
    dbg_info("\t[elems id:%s]~%d\n",$2, s_dbg_count++);
    FfNode *node = ff_create_id_node($2); 
    $$ = ff_link_node($1, node);
  }
  | elems list {
    dbg_info("\t[elems list]~%d\n", s_dbg_count++);
    $$ = ff_link_node($1, $2);
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

