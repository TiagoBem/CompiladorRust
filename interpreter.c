#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

void printExpr(Expr* expr, int tabs) {
  int i=0;
  if (expr == 0) {
    yyerror("Null expression!!");
  }
  else if (expr->kind == E_INTEGER) {
    for(i=0;i<tabs;i++)
      printf("\t");
    printf("%d\n",expr->attr.value);
  }
  else if (expr->kind == E_VAR) {
    for(i=0;i<tabs;i++)
      printf("\t");
    printf("%s\n",expr->attr.var);
    return;
  }
  else if (expr->kind == E_OPERATION) {
    for(i=0;i<tabs;i++)
      printf("\t");
    switch (expr->attr.op.operator) {
     case PLUS:
       printf("+:\n");
       break;
    case MINUS:
      printf("-:\n");
      break;
    case MULT:
      printf("*:\n");
      break;
    case DIV:
      printf("/:\n");
      break;
    case MOD:
      printf("%%:\n");
      break;
    }
    printExpr(expr->attr.op.left,tabs+1);
    printExpr(expr->attr.op.right,tabs+1);
  }
}

void evalBoolExpr(BoolExpr* expr, int tabs) {
  int i=0;
  if(expr->kind == E_BOOL_OPERATION) {
    switch(expr->attr.op.boolOp) {
      case EQUAL:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("EQUAL\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
      case NOT_EQUAL:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("NOT_EQUAL\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
      case LESSER:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("LESSER\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
      case BIGGER:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("BIGGER\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
      case LESSER_EQUAL:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("LESSER_EQUAL\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
      case BIGGER_EQUAL:
        for(i=0;i<tabs;i++)
          printf("\t");
        printf("BIGGER_EQUAL\n");
        printExpr(expr->attr.op.left,tabs+1);
        printExpr(expr->attr.op.right,tabs+1);
        break;
    }
  }
  else if(expr->kind == E_TRUEORFALSE){
    if(expr->attr.bool_value == 1){
      for(i=0;i<tabs;i++)
        printf("\t");
      printf("TRUE\n");
    }
    else {
      for(i=0;i<tabs;i++)
        printf("\t");
      printf("FALSE\n");
    }
  }
}


  void evalCmdList(CmdList* list,int tabs) {
    if(list == NULL)
      return;
    else if(list->cmd_list == NULL){
      evalCmd(list->cmd,tabs);
      return;
    }
    else{
      evalCmd(list->cmd,tabs);
      evalCmdList(list->cmd_list,tabs);
    }
    return;
  }

  void evalCmd(Cmd* list, int tabs) {
    int i=0;
    switch (list->kind) {
        case E_ATRIB:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("ATRIB\n");
            for(i=0;i<tabs+1;i++)
              printf("\t");
            printf ("%s\n", list->attr.atrib.var);
            printExpr(list->attr.atrib.value,tabs+1);
            break;
        case E_IF:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("IF\n");
            evalBoolExpr(list->attr._if.boolExpr,tabs+1);
            evalCmdList(list->attr._if.cmd_list, tabs+2);
            break;
        case E_IFELSE:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("IF\n");
            evalBoolExpr(list->attr._ifelse.boolExpr,tabs+1);
            evalCmdList(list->attr._ifelse.cmd_list1, tabs+2);
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("ELSE\n");
            evalCmdList(list->attr._ifelse.cmd_list2, tabs+1);
            break;
        case E_WHILE:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("WHILE\n");
            evalBoolExpr(list->attr._while.boolExpr, tabs+1);
            evalCmdList(list->attr._while.cmd_list, tabs+2);
            break;
        case E_PRINT:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("PRINT\n");
            for(i=0;i<tabs+1;i++)
              printf("\t");
            printf("%s\n",list->attr._print.str);
            break;
        case E_PRINTVAR:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf ("PRINT_VAR\n");
            for(i=0;i<tabs+1;i++)
              printf("\t");
            printf("%s\n", list->attr._printVar.var);
            for(i=0;i<tabs+1;i++)
              printf("\t");
            printf("%s\n", list->attr._printVar.str);
            break;
        case E_READ:
            for(i=0;i<tabs;i++)
              printf("\t");
            printf("READ\n");
            for(i=0;i<tabs+1;i++)
              printf("\t");
            printf("%s\n",list->attr._read.var);
            break;
    }
    return;
}





  int main(int argc, char** argv) {
    --argc; ++argv;
    if (argc != 0) {
      yyin = fopen(*argv, "r");
      if (!yyin) {
        printf("'%s': could not open file\n", *argv);
        return 1;
      }
    } //  yyin = stdin
    if (yyparse() == 0) {
      printf("MAIN\n");
      CmdList* list = root;
      evalCmdList(list,1);
    }
    return 0;
  }
