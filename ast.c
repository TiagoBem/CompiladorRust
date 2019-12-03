// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr* ast_integer(int v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_var(char* string) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VAR;
  node->attr.var = strdup(string);
  return node;
}

Expr* ast_operation
(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

BoolExpr* ast_TorF(int val){
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_TRUEORFALSE;
  node->attr.bool_value = val;
  return node;
}

BoolExpr* ast_BoolOperation(int operator, Expr* left, Expr* right) {
  BoolExpr* node = (BoolExpr*) malloc(sizeof(Expr));
  node->kind = E_BOOL_OPERATION;
  node->attr.op.boolOp = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

Cmd* ast_atrib(char* string, Expr* expr){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
	node->kind = E_ATRIB;
	node->attr.atrib.var = strdup(string);
	node->attr.atrib.value = expr;
	return node;
}

Cmd* ast_if(BoolExpr* boolExpr, CmdList* cmd){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_IF;
  node->attr._if.boolExpr = boolExpr;
  node->attr._if.cmd_list = cmd;
  return node;
}

Cmd* ast_ifelse(BoolExpr* boolExpr, CmdList* cmd1, CmdList* cmd2){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_IFELSE;
  node->attr._ifelse.boolExpr = boolExpr;
  node->attr._ifelse.cmd_list1 = cmd1;
  node->attr._ifelse.cmd_list2 = cmd2;
  return node;
}

Cmd* ast_while(BoolExpr* boolExpr, CmdList* cmd){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_WHILE;
  node->attr._while.boolExpr = boolExpr;
  node->attr._while.cmd_list = cmd;
  return node;
}

CmdList* ast_CmdList(Cmd* cmd1, CmdList* cmd2){
  CmdList* node = (CmdList*) malloc(sizeof(CmdList));
  node->cmd = cmd1;
  node->cmd_list = cmd2;
  return node;
}

Cmd* ast_print(char* string){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_PRINT;
  node->attr._print.str = strdup(string);
  return node;
}

Cmd* ast_printVar(char* string, char* var){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_PRINTVAR;
  node->attr._printVar.str = strdup(string);
  node->attr._printVar.var = strdup(var);
  return node;
}

Cmd* ast_read(char* string){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_READ;
  node->attr._read.var = strdup(string);
  return node;
}
