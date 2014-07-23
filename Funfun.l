%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "ast.h"
%}

%option noyywrap yylineno

ID  [a-zA-Z_+=~!@#$%&*\\\-\^]+
%%
"("       { return '('; }
")"       { return ')'; }
"quote"   { return QUOTE; }
"atom"    { return ATOM; }
"eq"      { return EQ; }
"car"     { return CAR; }
"cdr"     { return CDR; }
"cons"    { return CONS; }
"cond"    { return COND; }
{ID}      {
  yylval.str_val = strdup(yytext);
  return ID;
          }
%%

