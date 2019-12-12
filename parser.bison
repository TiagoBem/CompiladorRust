// Tokens
%token
  INT
  VAR
  STR
  PLUS
  MINUS
  MULT
  DIV
  MOD
  EQUAL
  NOT_EQUAL
  LESSER
  BIGGER
  LESSER_EQUAL
  BIGGER_EQUAL
  AND
  PV
  COMMA
  IF
  ELSE
  ATRIB
  ONE_EQUAL
  MAIN
  WHILE
  O_BRK
  C_BRK
  O_PAR
  C_PAR
  PRINT
  READ
  TRUE
  FALSE

// Operator associativity & precedence
%left PLUS MINUS
%left MULT DIV
%left MOD


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char* charValue;
  Expr* exprValue;
  BoolExpr* boolExprValue;
  char* str;
  Cmd* cmd;
  CmdList* cmd_list;
}

%type <intValue> INT
%type <charValue> VAR
%type <str> STR
%type <exprValue> expr
%type <cmd> cmd
%type <cmd_list> cmd_list
%type <boolExprValue> BoolExpr

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
}

%%
program: MAIN O_PAR C_PAR O_BRK cmd_list C_BRK { root = $5; }

cmd_list:
  {$$ = NULL; }
  |
  cmd cmd_list {
    $$ = ast_CmdList($1,$2);
  }
  ;

cmd:
    ATRIB VAR ONE_EQUAL expr PV {
        $$ = ast_atrib($2, $4);
    }
    |
    IF BoolExpr O_BRK cmd_list C_BRK {
		  $$ = ast_if($2, $4);
    }
    |
    IF BoolExpr O_BRK cmd_list C_BRK ELSE O_BRK cmd_list C_BRK{
        $$ = ast_ifelse($2,$4,$8);
    }
    |
    WHILE BoolExpr O_BRK cmd_list C_BRK {
        $$ = ast_while($2,$4);
    }
    |
    PRINT O_PAR STR C_PAR PV {
        $$ = ast_print($3);
    }
    |
    PRINT O_PAR STR COMMA VAR C_PAR PV {
        $$ = ast_printVar($3,$5);
    }
    |
    READ O_PAR AND VAR C_PAR PV {
        $$ = ast_read($4);
    }
    ;

expr:
  INT {
    $$ = ast_integer($1);
  }
  |
  VAR {
    $$ = ast_var($1);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr MINUS expr  {
    $$ = ast_operation(MINUS, $1, $3);
  }
  |
  expr MULT expr {
    $$ = ast_operation(MULT, $1, $3);
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3);
  }
  |
  expr MOD expr {
    $$ = ast_operation(MOD, $1, $3);
  }
  ;


BoolExpr:
  TRUE {
    $$ = ast_TorF(1);
  }
  |
  FALSE {
    $$ = ast_TorF(0);
  }
  |
  expr EQUAL expr {
    $$ = ast_BoolOperation(EQUAL, $1, $3);
  }
  |
  expr NOT_EQUAL expr  {
    $$ = ast_BoolOperation(NOT_EQUAL, $1, $3);
  }
  |
  expr LESSER expr {
    $$ = ast_BoolOperation(LESSER, $1, $3);
  }
  |
  expr BIGGER expr {
    $$ = ast_BoolOperation(BIGGER, $1, $3);
  }
  |
  expr LESSER_EQUAL expr {
    $$ = ast_BoolOperation(LESSER_EQUAL, $1, $3);
  }
  |
  expr BIGGER_EQUAL expr {
    $$ = ast_BoolOperation(BIGGER_EQUAL, $1, $3);
  }
  ;

%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
