%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "ast.h"
%}

%option noyywrap yylineno

ID  [a-zA-Z_+=~!@#$%&*"\\\-\^]+
%%
"("       { 
  dbg_info("( ");
  return '('; 
}
")"       { 
  dbg_info(") ");
  return ')'; 
}
"quote"   { return QUOTE; }
"atom"    { return ATOM; }
"eq"      { return EQ; }
"car"     { return CAR; }
"cdr"     { return CDR; }
"cons"    { return CONS; }
"cond"    { return COND; }
{ID}      {
  yylval.str_val = strdup(yytext);
  dbg_info("%s ", yylval.str_val);
  return ID;
          }
[ \t] {
  /*dbg_info(" BK ");*/
}

[\r\n]+ {
  dbg_info("\n");
}

. {
  dbg_info("???: ", strdup(yytext));
}

%%

