/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <stddef.h>

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct FuncDecl FuncDecl;
typedef struct ClassDecl ClassDecl;
typedef struct Program Program;

typedef enum {
    EXPR_INT_LIT,
    EXPR_STR_LIT,
    EXPR_ID,
    EXPR_NEW,
    EXPR_CAST,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_MEMBER,
    EXPR_CALL
} ExprKind;

typedef struct {
    Expr** items;
    size_t count;
    size_t capacity;
} ExprList;

struct Expr {
    ExprKind kind;
    int line;
    char* inferred_type; /* Rempli lors de l'analyse sémantique */
    union {
        int64_t int_val;                        /* EXPR_INT_LIT */
        char* str_val;                          /* EXPR_STR_LIT, EXPR_ID, EXPR_NEW */
        struct { char* target_type; Expr* expr; } cast;
        struct { char* op; Expr* operand; } unary;
        struct { char* op; Expr* left; Expr* right; } binary;
        struct { Expr* object; char* member; } member;
        struct { Expr* callee; ExprList args; } call;
    } as;
};

typedef enum {
    STMT_VAR_DECL,
    STMT_ASSIGN,
    STMT_IF,
    STMT_WHILE,
    STMT_RETURN,
    STMT_EXPR
} StmtKind;

typedef struct {
    char** items;
    size_t count;
    size_t capacity;
} StringList;

typedef struct {
    Stmt** items;
    size_t count;
    size_t capacity;
} StmtList;

struct Stmt {
    StmtKind kind;
    int line;
    union {
        struct { char* type; StringList names; } var_decl;
        struct { Expr* target; Expr* value; } assign;
        struct { Expr* cond; StmtList then_block; StmtList else_block; } if_stmt;
        struct { Expr* cond; StmtList body; } while_stmt;
        struct { Expr* value; } ret_stmt; /* value == NULL si "return;" */
        struct { Expr* expr; } expr_stmt;
    } as;
};

typedef struct {
    char* type;
    char* name;
} Param;

typedef struct {
    Param* items;
    size_t count;
    size_t capacity;
} ParamList;

struct FuncDecl {
    int line;
    char* return_type;
    char* name;
    ParamList params;
    StmtList body;
};

struct ClassDecl {
    int line;
    char* name;
    char* super_name;
    StmtList fields;       /* Liste de STMT_VAR_DECL */
    FuncDecl** methods;
    size_t method_count;
    size_t method_capacity;
};

struct Program {
    FuncDecl** functions;
    size_t func_count;
    size_t func_capacity;
    ClassDecl** classes;
    size_t class_count;
    size_t class_capacity;
};

/* Constructeurs AST */
Program* ast_program_new(void);
void ast_program_add_func(Program* p, FuncDecl* f);
void ast_program_add_class(Program* p, ClassDecl* c);

FuncDecl* ast_func_new(int line, char* ret_type, char* name, ParamList params, StmtList body);
ClassDecl* ast_class_new(int line);
void ast_class_add_field(ClassDecl* c, Stmt* field);
void ast_class_add_method(ClassDecl* c, FuncDecl* method);

void string_list_push(StringList* list, char* str);
void param_list_push(ParamList* list, char* type, char* name);
void stmt_list_push(StmtList* list, Stmt* stmt);
void expr_list_push(ExprList* list, Expr* expr);

Stmt* ast_stmt_var_decl(int line, char* type, StringList names);
Stmt* ast_stmt_assign(int line, Expr* target, Expr* value);
Stmt* ast_stmt_if(int line, Expr* cond, StmtList then_b, StmtList else_b);
Stmt* ast_stmt_while(int line, Expr* cond, StmtList body);
Stmt* ast_stmt_return(int line, Expr* value);
Stmt* ast_stmt_expr(int line, Expr* expr);

Expr* ast_expr_int(int line, int64_t val);
Expr* ast_expr_str(int line, char* val);
Expr* ast_expr_id(int line, char* name);
Expr* ast_expr_new(int line, char* class_name);
Expr* ast_expr_cast(int line, char* target_type, Expr* expr);
Expr* ast_expr_unary(int line, char* op, Expr* operand);
Expr* ast_expr_binary(int line, char* op, Expr* left, Expr* right);
Expr* ast_expr_member(int line, Expr* object, char* member);
Expr* ast_expr_call(int line, Expr* callee, ExprList args);

#endif