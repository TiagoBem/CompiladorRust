// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum {
    E_INTEGER,
    E_OPERATION,
    E_VAR
  } kind;
  union {
    int value; // for integer values
    struct {
      int operator; // PLUS, MINUS, etc
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
    char* var;
  } attr;
};


struct _BoolExpr {
  enum {
    E_TRUEORFALSE,
    E_BOOL_OPERATION
  } kind;
  union {
    int bool_value;
    struct {
      int boolOp;
      struct _Expr* left;
      struct _Expr* right;
    } op;
  } attr;
};


struct _Cmd {
  enum {
    E_ATRIB,
    E_IF,
    E_IFELSE,
    E_WHILE,
    E_PRINT,
    E_PRINTVAR,
    E_READ
  }kind;
  union{
    struct {
      char* var;
      struct _Expr* value;
    } atrib;
    struct {
      struct _BoolExpr* boolExpr;
      struct _CmdList* cmd_list;
    } _if;
    struct {
      struct _BoolExpr* boolExpr;
      struct _CmdList* cmd_list1;
      struct _CmdList* cmd_list2;
    } _ifelse;
    struct {
      struct _BoolExpr* boolExpr;
      struct _CmdList* cmd_list;
    } _while;
    struct {
      char* str;
    } _print;
    struct {
      char* str;
      char* var;
    } _printVar;
    struct {
      char* var;
    } _read;
  } attr;
};

struct _CmdList {
  struct _Cmd* cmd;
  struct _CmdList* cmd_list; 
};


typedef struct _Expr Expr; // Convenience typedef
typedef struct _BoolExpr BoolExpr;
typedef struct _Cmd Cmd;
typedef struct _CmdList CmdList;


// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_var(char* string);
Expr* ast_operation(int operator, Expr* left, Expr* right);
BoolExpr* ast_TorF(int val);
BoolExpr* ast_BoolOperation(int operator, Expr* left, Expr* right);
Cmd* ast_atrib(char* string, Expr* expr);
Cmd* ast_if(BoolExpr* boolExpr, CmdList* cmd);
Cmd* ast_ifelse(BoolExpr* boolExpr, CmdList* cmd1, CmdList* cmd2);
Cmd* ast_while(BoolExpr* boolExpr, CmdList* cmd);
CmdList* ast_CmdList(Cmd* cmd1, CmdList* cmd2);
Cmd* ast_print(char* string);
Cmd* ast_printVar(char* string, char* var);
Cmd* ast_read(char* string);



#endif
