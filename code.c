#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "ast.h"
#include "parser.h"

int regist=0;
char buffer[12];

char* create_register(){
    sprintf(buffer,"%s%d","t",regist);
    char* var = buffer;
    regist++;
    return var;
}

List_Instr* compile_cmdlist(CmdList* cmd_list);
List_Instr* compile_BoolExpr(BoolExpr* boolexpr);
List_Instr* compile_Cmd(Cmd* cmd);
List_Instr* compile_Expr(Expr* expr,char* r);

Atom* mkAtomInt(int n) {
    Atom* node = (Atom*) malloc(sizeof(Atom));
    node->kind = A_INT;
    node->num = n;
    return node;
}

Atom* mkAtomChar(char* c) {
    Atom* node = (Atom*) malloc(sizeof(Atom));
    node->kind = A_VAR;
    node->var = c;
    return node;
}


Instr* mkInstrOneAtom(iKind kind, char* var, Atom* atom) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._one_var.var = var;
    node->args._one_var.atom = atom;
    return node;
}

Instr* mkInstrTwoAtom(iKind kind, char* var, Atom* atom1, Atom* atom2) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._two_var.var = var;
    node->args._two_var.atom1 = atom1;
    node->args._two_var.atom2 = atom2;
    return node;
}

Instr* mkInstrGoto(iKind kind, Label* label) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._goto.label = label;
    return node;
}

Instr* mkInstrIfElse(iKind kind, char* var, Atom* atom, Label* label1, Label* label2) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._if_else.var = var;
    node->args._if_else.atom = atom;
    node->args._if_else.label1 = label1;
    node->args._if_else.label2 = label2;
    return node;
}

Instr* mkInstrLab(iKind kind, Label* label) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._lab.label = label;
    return node;
}

Instr* head(List_Instr* list) {
    return list->head;
}


List_Instr* tail(List_Instr* l) {
    return l->tail;
}

List_Instr* mkList(Instr* code, List_Instr* l) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    node->head = code;
    node->tail = l;
    return node;
}

List_Instr* append(List_Instr* l1, List_Instr* l2) {
    if(l1 == NULL)
        return l2;
    return mkList(head(l1), append(tail(l1), l2));
}

/*
List_Instr* compile_cmdlist(CmdList* cmd_list) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    node = compile_Cmd(cmd_list->cmd);
    while(cmd_list->cmd_list!=NULL){
        cmd_list = cmd_list->cmd_list;
        node = append(node, compile_Cmd(cmd_list->cmd));
    }
    return node;
}

List_Instr* compile_BoolExpr(BoolExpr* boolexpr) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    switch(boolexpr->kind){
        case E_TRUEORFALSE:
            switch (boolexpr->attr.bool_value) {
                case 1:
                    node = mkList(mkInstrOneAtom(TRU,mkAtomInt(boolexpr->attr.bool_value)), NULL);
                    break;
                case 0:
                    node = mkList(mkInstrOneAtom(FALS,mkAtomInt(boolexpr->attr.bool_value)), NULL);
                    break;
            }
        case E_BOOL_OPERATION:
            node = compile_Expr(boolexpr->attr.op.left);
            node = append(node, compile_Expr(boolexpr->attr.op.right));
            switch (boolexpr->attr.op.boolOp) {
                case EQUAL:
                    node = append(node, mkList(mkInstrTwoAtom(EQU,0), NULL));
                    break;
                case NOT_EQUAL:
                    node = append(node, mkList(mkInstrTwoAtom(N_EQU,0), NULL));
                    break;
                case LESSER:
                    node = append(node, mkList(mkInstrTwoAtom(LES,0), NULL));
                    break;
                case BIGGER:
                    node = append(node, mkList(mkInstrTwoAtom(BIG,0), NULL));
                    break;
                case LESSER_EQUAL:
                    node = append(node, mkList(mkInstrTwoAtom(LEQ,0), NULL));
                    break;
                case BIGGER_EQUAL:
                    node = append(node, mkList(mkInstrOneAtom(BEQ,0), NULL));
                    break;
            }
    }
    return node;
}


List_Instr* compile_Cmd(Cmd* cmd) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    switch(cmd->kind){
        case E_ATRIB:
            break;
        case E_IF:
            break;
        case E_IFELSE:
            break;
        case E_WHILE:
            break;
        case E_PRINT:
            break;
        case E_PRINTVAR:
            break;
        case E_READ:
            break;
        default:
            yyerror("Unknown command!\n");
    }
    return node;
}*/

List_Instr* compile_Expr(Expr* expr, char* r) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    char* r1;
    char* r2;
    printf("%s\n",r);
    switch(expr->kind){
        case E_INTEGER:
            node = mkList(mkInstrOneAtom(NUM,r,mkAtomInt(expr->attr.value)),NULL);
            break;
        case E_VAR:
            node = mkList(mkInstrOneAtom(VARIAB,r,mkAtomChar(expr->attr.var)),NULL);
            break;
        case E_OPERATION:
            r1 = create_register();
            //printf("left %d\n",expr->attr.op.left->attr.value);
            node = compile_Expr(expr->attr.op.left,r1);
            r2 = create_register();
            //printf("right %d\n",expr->attr.op.right->attr.value);
            node = append(node, compile_Expr(expr->attr.op.right,r2));
            printf("reg %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.var);
            printf("val atom1 %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.atom1->var);
            printf("val atom2 %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.atom2->var);
            switch(expr->attr.op.operator){
                case PLUS:
                    node = append(node, mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case MINUS:
                    node = append(node, mkList(mkInstrTwoAtom(SUB,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case MULT:
                    node = append(node, mkList(mkInstrTwoAtom(MUL,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case DIV:
                    node = append(node, mkList(mkInstrTwoAtom(DIVI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
            }
    }
    printf("registo var  %s\n",node->head->args._two_var.var);
    if(node->head->args._two_var.atom1 != NULL)
        printf("registo atom1 %d\n",node->head->args._two_var.atom1->num);
    if(node->head->args._two_var.atom2 != NULL)
        printf("registo atom2 %d\n",node->head->args._two_var.atom2->num);
    return node;
}


void printInstr(Instr* inst){
    //printf("%d\n",inst->kind);
  switch(inst->kind){
    case NUM:
        //printf("NUM");
        printf("%s",inst->args._two_var.var);
        printf(" := %d\n", inst->args._one_var.atom->num);
    break;
    case VARIAB:
        //printf("VARIAB");
        printf("%s\n", inst->args._one_var.var);
        printf(" := %s\n", inst->args._one_var.atom->var);
        break;
    case ADI:
        //printf("ADI\n");
        //printf("%d\n",inst->args._two_var.atom1->num);
        //printf("%d\n",inst->args._two_var.atom2->num);
        printf("%s",inst->args._two_var.var);
        printf(" := %s + %s\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case SUB:
        printf("SUB\n");
        break;
    case MUL:
        printf("MUL\n");
        break;
    case DIVI:
        printf("DIVI\n");
        break;
    case GOTO:
        printf("GOTO\n");
        break;
    case LAB:
        printf("LAB\n");
        break;
    case TRU:
        printf("TRU\n");
        break;
    case FALS:
        printf("FALS\n");
        break;
    case EQU:
        printf("EQU\n");
        break;
    case N_EQU:
        printf("N_EQU\n");
        break;
    case LES:
        printf("LES\n");
        break;
    case BIG:
        printf("BIG\n");
        break;
    case LEQ:
        printf("LEQ\n");
        break;
    case BEQ:
        printf("BEQ\n");
        break;
    default: printf("Não existe\n");
  }
}

void printListInstr(List_Instr* list){
    if(list == NULL)
        return;
    if(list->tail == NULL){
        printInstr(list->head);
        return;
    }
    //printf("head  %d\n",list->head);
    printInstr(list->head);
    //printf("tail  %d\n",list->tail);
    printListInstr(list->tail);
    return;
}

/*
int main(int argc, char** argv) {
    argc--;
    argv++;
    if(argc!=0) {
        yyin=fopen(*argv,"r");
        if(!yyin) {
            printf("'%s':could not open file\n", *argv);
            return 1;
        }
    }
    if(yyparse()==0) {
        printf("%s\n",root->kind);
        //List_Instr* list = compile_Expr(root);
        Expr* expr1 = ast_integer(1);
        Expr* expr2 = ast_integer(2);
        Expr* res = ast_operation(PLUS,expr1,expr2);
        List_Instr* list = compile_Expr(res);
        printf("fez compile\n");
        //printListInstr(list);
        printf("SYSCALL\n");
    }
    return 0;
}*/

int main() {
    Expr* expr1 = ast_integer(1);
    Expr* expr2 = ast_integer(2);
    Expr* res = ast_operation(PLUS,expr1,expr2);
    char* r = create_register();
    List_Instr* list = compile_Expr(res,r);
    printf("SYSCALL\n");
    printListInstr(list);

    return 0;
}