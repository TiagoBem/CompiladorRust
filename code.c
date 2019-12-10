#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "ast.h"
#include "parser.h"

int regist=0;

char* create_register(){
    char* var = (char*) malloc(sizeof(char));
    sprintf(var,"%s%d","t",regist);
    regist++;
    return var;
}

List_Instr* compile_cmdlist(CmdList* cmd_list);
List_Instr* compile_BoolExpr(BoolExpr* boolexpr, char* r, Label* labelTrue, Label* labelFalse);
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
    node->var = strdup(c);
    return node;
}


Instr* mkInstrOneAtom(iKind kind, char* var, Atom* atom) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._one_var.var = strdup(var);
    node->args._one_var.atom = atom;
    return node;
}

Instr* mkInstrTwoAtom(iKind kind, char* var, Atom* atom1, Atom* atom2) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._two_var.var = strdup(var);
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
    node->args._if_else.var = strdup(var);
    node->args._if_else.atom = atom;
    node->args._if_else.label1 = label1;
    node->args._if_else.label2 = label2;
    return node;
}

Instr* mkInstrTrueFalse(iKind kind, char* var, char* TorF, Label* label1, Label* label2) {
    Instr* node = (Instr*) malloc(sizeof(Instr));
    node->kind = kind;
    node->args._if_else.var = strdup(var);
    node->args._if_else.TorF = strdup(TorF);
    node->args._if_else.label1 = label1;
    node->args._if_else.label2 = label2;
    return node;
}

Label* mkLab(char* var, List_Instr* list) {
    Label* node = (Label*) malloc(sizeof(Label));
    node->var = var;
    node->list_instr = list;
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
*/

List_Instr* compile_BoolExpr(BoolExpr* boolexpr, char* r, Label* labelTrue, Label* labelFalse) {
    List_Instr* node = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp1 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp2 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp3 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp4 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp5 = (List_Instr*) malloc(sizeof(List_Instr));    
    List_Instr* temp6 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp7 = (List_Instr*) malloc(sizeof(List_Instr));
    char* r1;
    char* r2;
    switch(boolexpr->kind){
        case E_TRUEORFALSE:
            switch (boolexpr->attr.bool_value) {
                case 1:
                    node = mkList(mkInstrOneAtom(TRU,r,mkAtomInt(boolexpr->attr.bool_value)), NULL);
                    break;
                case 0:
                    node = mkList(mkInstrOneAtom(FALS,r,mkAtomInt(boolexpr->attr.bool_value)), NULL);
                    break;
            }
        case E_BOOL_OPERATION:
            r1 = create_register();
            temp1 = compile_Expr(boolexpr->attr.op.left,r1);
            r2 = create_register();
            temp2 = compile_Expr(boolexpr->attr.op.right,r2);
            temp3 = append(temp1,temp2);
            r1 = temp1->head->args._one_var.var;
            r2 = temp2->head->args._one_var.var;
            switch (boolexpr->attr.op.boolOp) {
                case EQUAL:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(EQU,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case NOT_EQUAL:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(N_EQU,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case LESSER:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(LES,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case BIGGER:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(BIG,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case LESSER_EQUAL:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(LEQ,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case BIGGER_EQUAL:
                    temp4 = append(temp3, mkList(mkInstrTwoAtom(BEQ,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
            }
    }
    temp5 = mkList(mkInstrGoto(LAB,labelTrue),NULL);
    temp6 = mkList(mkInstrGoto(LAB,labelFalse),NULL);
    temp7 = append(temp5,temp6);
    node = append(temp4,temp7);
    return node;
}

/*
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
    List_Instr* temp1 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp2 = (List_Instr*) malloc(sizeof(List_Instr));
    List_Instr* temp3 = (List_Instr*) malloc(sizeof(List_Instr));
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
            temp1 = compile_Expr(expr->attr.op.left,r1);
            r2 = create_register();
            //printf("right %d\n",expr->attr.op.right->attr.value);
            temp2 = compile_Expr(expr->attr.op.right,r2);
            temp3 = append(temp1,temp2);
            printf("reg %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.var);
            printf("val atom1 %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.atom1->var);
            printf("val atom2 %s\n",(mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL))->head->args._two_var.atom2->var);
            switch(expr->attr.op.operator){
                case PLUS:
                    node = append(temp3, mkList(mkInstrTwoAtom(ADI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case MINUS:
                    node = append(temp3, mkList(mkInstrTwoAtom(SUB,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case MULT:
                    node = append(temp3, mkList(mkInstrTwoAtom(MUL,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
                    break;
                case DIV:
                    node = append(temp3, mkList(mkInstrTwoAtom(DIVI,r,mkAtomChar(r1),mkAtomChar(r2)), NULL));
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
        printf("%s",inst->args._one_var.var);
        printf(" := %d\n", inst->args._one_var.atom->num);
    break;
    case VARIAB:
        printf("%s", inst->args._one_var.var);
        printf(" := %s\n", inst->args._one_var.atom->var);
        break;
    case ADI:
        printf("%s",inst->args._two_var.var);
        printf(" := %s + %s\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case SUB:
        printf("%s",inst->args._two_var.var);
        printf(" := %s - %s\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case MUL:
        printf("%s",inst->args._two_var.var);
        printf(" := %s * %s\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case DIVI:
        printf("%s",inst->args._two_var.var);
        printf(" := %s / %s\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case LAB:
        printf("Label %s:\n",inst->args._goto.label->var);
        printListInstr(inst->args._goto.label->list_instr);
        break;
    case TRU:
        printf("TRU\n");
        break;
    case FALS:
        printf("FALS\n");
        break;
    case EQU:
        printf("if %s == %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case N_EQU:
        printf("if %s != %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case LES:
        printf("if %s < %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case BIG:
        printf("if %s > %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case LEQ:
        printf("if %s <= %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
        break;
    case BEQ:
        printf("if %s >= %s go to\n", inst->args._two_var.atom1->var, inst->args._two_var.atom2->var);
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
        //List_Instr* list = compile_Expr(root);
        /*Expr* expr1 = ast_integer(1);
        Expr* expr2 = ast_integer(2);
        Expr* res = ast_operation(PLUS,expr1,expr2);*/
        char* r = create_register();
        List_Instr* list = compile_Expr(root,r);
        printf("SYSCALL\n");
        printListInstr(list);
    }
    return 0;
}
/*
int main() {
    Expr* expr1 = ast_integer(2);
    Expr* expr2 = ast_integer(5);
    Expr* expr3 = ast_operation(MULT,expr1,expr2);
    char* var = (char*) malloc(sizeof(char));
    var = "x";
    Expr* expr4 = ast_var(var);
    Expr* res = ast_operation(MINUS,expr4,expr3);
    BoolExpr* be = ast_BoolOperation(EQUAL,expr1,expr2);
    char* r1 = create_register();
    List_Instr* list1 = compile_Expr(res,r1);
    char* r2 = create_register();
    char* a = "a";
    Label* l1 = mkLab(a,list1);
    char* b = "b";
    Label* l2 = mkLab(b,list1);
    List_Instr* list2 = compile_BoolExpr(be,r2,l1,l2); 
    printf("SYSCALL\n");
    printListInstr(list2);

    return 0;
}*/