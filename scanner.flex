%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return INT;
}

"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return MOD; }
"==" { return EQUAL; }
"!=" { return NOT_EQUAL; }
"<" { return LESSER; }
">" { return BIGGER; }
"<=" { return LESSER_EQUAL; }
">=" { return BIGGER_EQUAL; }
"if" { return IF; }
"else" { return ELSE; }
"let" { return ATRIB; }
"=" { return ONE_EQUAL; }
"fn main" { return MAIN; }
"while" { return WHILE; }
"true" { return TRUE; }
"false" { return FALSE; }
"{" { return O_BRK; }
"}" { return C_BRK; }
"(" { return O_PAR; }
")" { return C_PAR; }
"println!" { return PRINT; }
"read_line" { return READ; }
"&" { return AND; }
";" { return PV; }
"," { return COMMA; }

[a-z]([a-zA-Z])* {
  yylval.charValue = strdup(yytext);
  return VAR;
}

\".*\" {
  yylval.str = strdup(yytext);
  return STR;
}

.  { yyerror("unexpected character"); }
%%
