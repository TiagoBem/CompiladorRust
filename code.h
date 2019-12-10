typedef enum {NUM,VARIAB,ADI,SUB,MUL,DIVI,LAB,TRU,FALS,EQU,N_EQU,LES,BIG,LEQ,BEQ} iKind;

struct _Atom {
    enum {
        A_VAR,
        A_INT
    } kind;
    char* var;
    int num;
};

struct _Label {
    char* var;
    struct _List_Instr* list_instr;
};

struct  _Instr{
    iKind kind;
    union {
        struct {
            char* var;
            struct _Atom* atom;
        } _one_var;
        struct {
            char* var;
            struct _Atom* atom1;
            struct _Atom* atom2;
        } _two_var;
        struct {
            struct _Label* label;
        } _goto;
        struct {
            char* var;
            struct _Atom* atom;
            char* TorF;
            struct _Label* label1;
            struct _Label* label2;
        } _if_else;
    } args;
};


struct _List_Instr{
    struct _Instr* head;
    struct _List_Instr* tail; 
};

typedef struct _Label Label;
typedef struct _Atom Atom;
typedef struct _Instr Instr;
typedef struct _List_Instr List_Instr;

Atom* mkAtomInt(int n);
Atom* mkAtomChar(char*c);

Instr* mkInstrOneAtom(iKind kind, char* var, Atom* atom);
Instr* mkInstrTwoAtom(iKind kind, char* var, Atom* atom1, Atom* atom2);
Instr* mkInstrGoto(iKind, Label* label);
Instr* mkInstrIfElse(iKind kind, char* var, Atom* atom, Label* label1, Label* label2);
Instr* mkInstrTrueFalse(iKind kind, char* var, char* TorF, Label* label1, Label* label2);
Instr* head(List_Instr* list);

Label* mkLab(char* var, List_Instr* list);

List_Instr* tail(List_Instr* l);
List_Instr* mkList(Instr* code, List_Instr* l);
List_Instr* append(List_Instr* l1, List_Instr* l2);