/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

static void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Internal error: out of memory\n");
        exit(19);
    }
    return ptr;
}

static void* xrealloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        fprintf(stderr, "Internal error: out of memory\n");
        exit(19);
    }
    return new_ptr;
}

Program* ast_program_new(void) {
    Program* p = xmalloc(sizeof(Program));
    p->functions = NULL; p->func_count = 0; p->func_capacity = 0;
    p->classes = NULL; p->class_count = 0; p->class_capacity = 0;
    return p;
}

void ast_program_add_func(Program* p, FuncDecl* f) {
    if (p->func_count == p->func_capacity) {
        p->func_capacity = p->func_capacity ? p->func_capacity * 2 : 8;
        p->functions = xrealloc(p->functions, p->func_capacity * sizeof(FuncDecl*));
    }
    p->functions[p->func_count++] = f;
}

void ast_program_add_class(Program* p, ClassDecl* c) {
    if (p->class_count == p->class_capacity) {
        p->class_capacity = p->class_capacity ? p->class_capacity * 2 : 8;
        p->classes = xrealloc(p->classes, p->class_capacity * sizeof(ClassDecl*));
    }
    p->classes[p->class_count++] = c;
}

FuncDecl* ast_func_new(int line, char* ret_type, char* name, ParamList params, StmtList body) {
    FuncDecl* f = xmalloc(sizeof(FuncDecl));
    f->line = line;
    f->return_type = ret_type;
    f->name = name;
    f->params = params;
    f->body = body;
    return f;
}

ClassDecl* ast_class_new(int line) {
    ClassDecl* c = xmalloc(sizeof(ClassDecl));
    c->line = line;
    c->name = NULL;
    c->super_name = NULL;
    c->fields = (StmtList){NULL, 0, 0};
    c->methods = NULL; c->method_count = 0; c->method_capacity = 0;
    return c;
}

void ast_class_add_field(ClassDecl* c, Stmt* field) {
    stmt_list_push(&c->fields, field);
}

void ast_class_add_method(ClassDecl* c, FuncDecl* method) {
    if (c->method_count == c->method_capacity) {
        c->method_capacity = c->method_capacity ? c->method_capacity * 2 : 8;
        c->methods = xrealloc(c->methods, c->method_capacity * sizeof(FuncDecl*));
    }
    c->methods[c->method_count++] = method;
}

void string_list_push(StringList* list, char* str) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->items = xrealloc(list->items, list->capacity * sizeof(char*));
    }
    list->items[list->count++] = str;
}

void param_list_push(ParamList* list, char* type, char* name) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->items = xrealloc(list->items, list->capacity * sizeof(Param));
    }
    list->items[list->count++] = (Param){type, name};
}

void stmt_list_push(StmtList* list, Stmt* stmt) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 8;
        list->items = xrealloc(list->items, list->capacity * sizeof(Stmt*));
    }
    list->items[list->count++] = stmt;
}

void expr_list_push(ExprList* list, Expr* expr) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->items = xrealloc(list->items, list->capacity * sizeof(Expr*));
    }
    list->items[list->count++] = expr;
}

static Stmt* alloc_stmt(StmtKind kind, int line) {
    Stmt* s = xmalloc(sizeof(Stmt));
    s->kind = kind;
    s->line = line;
    return s;
}

Stmt* ast_stmt_var_decl(int line, char* type, StringList names) {
    Stmt* s = alloc_stmt(STMT_VAR_DECL, line);
    s->as.var_decl.type = type;
    s->as.var_decl.names = names;
    return s;
}

Stmt* ast_stmt_assign(int line, Expr* target, Expr* value) {
    Stmt* s = alloc_stmt(STMT_ASSIGN, line);
    s->as.assign.target = target;
    s->as.assign.value = value;
    return s;
}

Stmt* ast_stmt_if(int line, Expr* cond, StmtList then_b, StmtList else_b) {
    Stmt* s = alloc_stmt(STMT_IF, line);
    s->as.if_stmt.cond = cond;
    s->as.if_stmt.then_block = then_b;
    s->as.if_stmt.else_block = else_b;
    return s;
}

Stmt* ast_stmt_while(int line, Expr* cond, StmtList body) {
    Stmt* s = alloc_stmt(STMT_WHILE, line);
    s->as.while_stmt.cond = cond;
    s->as.while_stmt.body = body;
    return s;
}

Stmt* ast_stmt_return(int line, Expr* value) {
    Stmt* s = alloc_stmt(STMT_RETURN, line);
    s->as.ret_stmt.value = value;
    return s;
}

Stmt* ast_stmt_expr(int line, Expr* expr) {
    Stmt* s = alloc_stmt(STMT_EXPR, line);
    s->as.expr_stmt.expr = expr;
    return s;
}

static Expr* alloc_expr(ExprKind kind, int line) {
    Expr* e = xmalloc(sizeof(Expr));
    e->kind = kind;
    e->line = line;
    e->inferred_type = NULL;
    return e;
}

Expr* ast_expr_int(int line, int64_t val) {
    Expr* e = alloc_expr(EXPR_INT_LIT, line);
    e->as.int_val = val;
    return e;
}

Expr* ast_expr_str(int line, char* val) {
    Expr* e = alloc_expr(EXPR_STR_LIT, line);
    e->as.str_val = val;
    return e;
}

Expr* ast_expr_id(int line, char* name) {
    Expr* e = alloc_expr(EXPR_ID, line);
    e->as.str_val = name;
    return e;
}

Expr* ast_expr_new(int line, char* class_name) {
    Expr* e = alloc_expr(EXPR_NEW, line);
    e->as.str_val = class_name;
    return e;
}

Expr* ast_expr_cast(int line, char* target_type, Expr* expr) {
    Expr* e = alloc_expr(EXPR_CAST, line);
    e->as.cast.target_type = target_type;
    e->as.cast.expr = expr;
    return e;
}

Expr* ast_expr_unary(int line, char* op, Expr* operand) {
    Expr* e = alloc_expr(EXPR_UNARY, line);
    e->as.unary.op = op;
    e->as.unary.operand = operand;
    return e;
}

Expr* ast_expr_binary(int line, char* op, Expr* left, Expr* right) {
    Expr* e = alloc_expr(EXPR_BINARY, line);
    e->as.binary.op = op;
    e->as.binary.left = left;
    e->as.binary.right = right;
    return e;
}

Expr* ast_expr_member(int line, Expr* object, char* member) {
    Expr* e = alloc_expr(EXPR_MEMBER, line);
    e->as.member.object = object;
    e->as.member.member = member;
    return e;
}

Expr* ast_expr_call(int line, Expr* callee, ExprList args) {
    Expr* e = alloc_expr(EXPR_CALL, line);
    e->as.call.callee = callee;
    e->as.call.args = args;
    return e;
}