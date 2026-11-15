/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtranke00) 
 */

#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sem_error(int code, int line, const char* msg) {
    fprintf(stderr, "Semantic error %d (line %d): %s\n", code, line, msg);
    exit(code);
}

static bool signatures_match(FuncInfo* a, FuncInfo* b) {
    if (strcmp(a->return_type, b->return_type) != 0) return false;
    if (a->params.count != b->params.count) return false;
    for (size_t i = 0; i < a->params.count; i++) {
        if (strcmp(a->params.items[i].type, b->params.items[i].type) != 0) return false;
    }
    return true;
}

/* Résolution récursive de l'héritage et construction de la vtable / offsets des champs */
static void resolve_class_hierarchy(SymTable* st, ClassInfo* cls) {
    if (cls->hierarchy_resolved) return;
    if (cls->visiting) {
        sem_error(14, cls->ast_node->line, "Cyclic class inheritance detected");
    }
    cls->visiting = true;

    ClassInfo* sup = symtab_find_class(st, cls->super_name);
    if (!sup) {
        sem_error(14, cls->ast_node->line, "Unknown superclass");
    }
    resolve_class_hierarchy(st, sup);
    cls->super_class = sup;

    /* 1. Hériter des champs du parent (conserve les mêmes offsets) */
    for (size_t i = 0; i < sup->field_count; i++) {
        class_add_field(cls, sup->fields[i].name, sup->fields[i].type);
    }

    /* 2. Ajouter les nouveaux champs de cette classe */
    ClassDecl* decl = cls->ast_node;
    for (size_t i = 0; i < decl->fields.count; i++) {
        Stmt* fstmt = decl->fields.items[i];
        const char* ftype = fstmt->as.var_decl.type;
        if (!symtab_type_exists(st, ftype)) {
            sem_error(14, fstmt->line, "Unknown field type");
        }
        for (size_t j = 0; j < fstmt->as.var_decl.names.count; j++) {
            const char* fname = fstmt->as.var_decl.names.items[j];
            if (strcmp(fname, "this") == 0 || strcmp(fname, "super") == 0) {
                sem_error(14, fstmt->line, "Reserved identifier used as field name");
            }
            if (class_find_field(cls, fname) != NULL) {
                sem_error(14, fstmt->line, "Duplicate or shadowed field in class hierarchy");
            }
            class_add_field(cls, fname, ftype);
        }
    }

    /* 3. Hériter de la Method Table du parent */
    for (size_t i = 0; i < sup->method_count; i++) {
        class_set_or_add_method(cls, sup->methods[i]);
    }

    /* 4. Ajouter ou redéfinir les méthodes de cette classe */
    for (size_t i = 0; i < decl->method_count; i++) {
        FuncDecl* mdecl = decl->methods[i];
        if (strcmp(mdecl->name, "this") == 0 || strcmp(mdecl->name, "super") == 0) {
            sem_error(14, mdecl->line, "Reserved identifier used as method name");
        }
        if (strcmp(mdecl->return_type, "void") != 0 && !symtab_type_exists(st, mdecl->return_type)) {
            sem_error(14, mdecl->line, "Unknown method return type");
        }
        /* Interdire les doublons au sein de la même classe (pas de surcharge) */
        for (size_t k = 0; k < i; k++) {
            if (strcmp(decl->methods[k]->name, mdecl->name) == 0) {
                sem_error(14, mdecl->line, "Method overloading/redefinition in same class is forbidden");
            }
        }
        for (size_t p = 0; p < mdecl->params.count; p++) {
            if (!symtab_type_exists(st, mdecl->params.items[p].type)) {
                sem_error(14, mdecl->line, "Unknown method parameter type");
            }
        }

        FuncInfo* minfo = malloc(sizeof(FuncInfo));
        if (!minfo) exit(19);
        *minfo = (FuncInfo){
            strdup(mdecl->name), strdup(mdecl->return_type), mdecl->params,
            false, true, strdup(cls->name), -1, 0
        };

        FuncInfo* existing = class_find_method(cls, mdecl->name);
        if (existing) {
            if (!signatures_match(existing, minfo)) {
                sem_error(14, mdecl->line, "Overridden method signature does not match parent method");
            }
        }
        class_set_or_add_method(cls, minfo);
    }

    cls->visiting = false;
    cls->hierarchy_resolved = true;
}

static const char* analyze_expr(SymTable* st, ClassInfo* cur_class, Expr* e);

static bool is_assignable(SymTable* st, const char* target_type, const char* expr_type) {
    if (strcmp(target_type, expr_type) == 0) return true;
    return symtab_is_subtype(st, expr_type, target_type);
}

static const char* analyze_call(SymTable* st, ClassInfo* cur_class, Expr* e) {
    Expr* callee = e->as.call.callee;
    ExprList* args = &e->as.call.args;

    /* Cas 1 : Appel de fonction globale (ou de méthode implicite si applicable, ici globale) */
    if (callee->kind == EXPR_ID) {
        const char* fname = callee->as.str_val;
        if (strcmp(fname, "this") == 0 || strcmp(fname, "super") == 0) {
            sem_error(14, e->line, "Cannot call 'this' or 'super' directly as a function");
        }
        FuncInfo* fn = symtab_find_func(st, fname);
        if (!fn) {
            sem_error(14, e->line, "Call to undefined function");
        }

        /* Cas spécial : print(...) variadique (au moins 1 argument int ou string) */
        if (strcmp(fname, "print") == 0) {
            if (args->count == 0) {
                sem_error(14, e->line, "print() requires at least one argument");
            }
            for (size_t i = 0; i < args->count; i++) {
                const char* atype = analyze_expr(st, cur_class, args->items[i]);
                if (strcmp(atype, "int") != 0 && strcmp(atype, "string") != 0) {
                    sem_error(13, args->items[i]->line, "print() only accepts int or string arguments");
                }
            }
            e->inferred_type = strdup("void");
            return e->inferred_type;
        }

        if (args->count != fn->params.count) {
            sem_error(14, e->line, "Incorrect number of arguments in function call");
        }
        for (size_t i = 0; i < args->count; i++) {
            const char* atype = analyze_expr(st, cur_class, args->items[i]);
            if (!is_assignable(st, fn->params.items[i].type, atype)) {
                sem_error(13, args->items[i]->line, "Argument type mismatch in function call");
            }
        }
        e->inferred_type = strdup(fn->return_type);
        return e->inferred_type;
    }

    /* Cas 2 : Appel de méthode obj.method(...) ou super.method(...) */
    if (callee->kind == EXPR_MEMBER) {
        Expr* obj = callee->as.member.object;
        const char* mname = callee->as.member.member;
        ClassInfo* target_cls = NULL;

        if (obj->kind == EXPR_ID && strcmp(obj->as.str_val, "super") == 0) {
            if (!cur_class || !cur_class->super_class) {
                sem_error(14, e->line, "'super' used outside of a valid subclass method");
            }
            target_cls = cur_class->super_class;
            obj->inferred_type = strdup(target_cls->name);
        } else {
            const char* obj_type = analyze_expr(st, cur_class, obj);
            target_cls = symtab_find_class(st, obj_type);
            if (!target_cls) {
                sem_error(13, e->line, "Method call on non-object type");
            }
        }

        FuncInfo* method = class_find_method(target_cls, mname);
        if (!method) {
            sem_error(14, e->line, "Call to undefined method");
        }
        if (args->count != method->params.count) {
            sem_error(14, e->line, "Incorrect number of arguments in method call");
        }
        for (size_t i = 0; i < args->count; i++) {
            const char* atype = analyze_expr(st, cur_class, args->items[i]);
            if (!is_assignable(st, method->params.items[i].type, atype)) {
                sem_error(13, args->items[i]->line, "Argument type mismatch in method call");
            }
        }
        e->inferred_type = strdup(method->return_type);
        return e->inferred_type;
    }

    sem_error(14, e->line, "Invalid callee expression");
    return NULL;
}

static const char* analyze_expr(SymTable* st, ClassInfo* cur_class, Expr* e) {
    switch (e->kind) {
        case EXPR_INT_LIT:
            e->inferred_type = strdup("int");
            return e->inferred_type;

        case EXPR_STR_LIT:
            e->inferred_type = strdup("string");
            return e->inferred_type;

        case EXPR_ID: {
            const char* name = e->as.str_val;
            if (strcmp(name, "super") == 0) {
                sem_error(14, e->line, "'super' can only be used for member/method access");
            }
            if (strcmp(name, "this") == 0) {
                if (!cur_class) sem_error(14, e->line, "'this' used outside of a class method");
                e->inferred_type = strdup(cur_class->name);
                return e->inferred_type;
            }
            /* 1. Chercher dans les variables locales / paramètres */
            VarSymbol* var = symtab_lookup_var(st, name);
            if (var) {
                e->inferred_type = strdup(var->type);
                return e->inferred_type;
            }
            /* 2. Si dans une méthode, chercher dans les attributs de la classe */
            if (cur_class) {
                FieldInfo* fld = class_find_field(cur_class, name);
                if (fld) {
                    e->inferred_type = strdup(fld->type);
                    return e->inferred_type;
                }
            }
            sem_error(14, e->line, "Undeclared identifier");
            return NULL;
        }

        case EXPR_NEW: {
            ClassInfo* cls = symtab_find_class(st, e->as.str_val);
            if (!cls) sem_error(14, e->line, "Instantiation of undefined class");
            e->inferred_type = strdup(cls->name);
            return e->inferred_type;
        }

        case EXPR_CAST: {
            const char* src = analyze_expr(st, cur_class, e->as.cast.expr);
            const char* dst = e->as.cast.target_type;
            if (!symtab_type_exists(st, dst)) {
                sem_error(14, e->line, "Cast to undefined type");
            }
            /* Seuls (string) int et les casts entre classes (downcast/upcast) sont valides */
            if (strcmp(dst, "string") == 0 && strcmp(src, "int") == 0) {
                e->inferred_type = strdup("string");
                return e->inferred_type;
            }
            if (symtab_find_class(st, dst) && symtab_find_class(st, src)) {
                if (symtab_is_subtype(st, src, dst) || symtab_is_subtype(st, dst, src)) {
                    e->inferred_type = strdup(dst);
                    return e->inferred_type;
                }
            }
            sem_error(13, e->line, "Invalid type cast");
            return NULL;
        }

        case EXPR_UNARY: {
            const char* t = analyze_expr(st, cur_class, e->as.unary.operand);
            if (strcmp(t, "int") != 0) {
                sem_error(13, e->line, "Logical NOT '!' requires an int operand");
            }
            e->inferred_type = strdup("int");
            return e->inferred_type;
        }

        case EXPR_BINARY: {
            const char* lt = analyze_expr(st, cur_class, e->as.binary.left);
            const char* rt = analyze_expr(st, cur_class, e->as.binary.right);
            const char* op = e->as.binary.op;

            if (strcmp(op, "+") == 0) {
                if (strcmp(lt, "int") == 0 && strcmp(rt, "int") == 0) {
                    e->inferred_type = strdup("int");
                } else if (strcmp(lt, "string") == 0 && strcmp(rt, "string") == 0) {
                    e->inferred_type = strdup("string");
                } else {
                    sem_error(13, e->line, "Operator '+' requires two ints or two strings");
                }
                return e->inferred_type;
            }
            if (strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
                strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
                if (strcmp(lt, "int") != 0 || strcmp(rt, "int") != 0) {
                    sem_error(13, e->line, "Arithmetic/logical operator requires int operands");
                }
                e->inferred_type = strdup("int");
                return e->inferred_type;
            }
            if (strcmp(op, "<") == 0 || strcmp(op, "<=") == 0 ||
                strcmp(op, ">") == 0 || strcmp(op, ">=") == 0) {
                if ((strcmp(lt, "int") == 0 && strcmp(rt, "int") == 0) ||
                    (strcmp(lt, "string") == 0 && strcmp(rt, "string") == 0)) {
                    e->inferred_type = strdup("int");
                    return e->inferred_type;
                }
                sem_error(13, e->line, "Relational operator requires matching int or string operands");
            }
            if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
                if ((strcmp(lt, "int") == 0 && strcmp(rt, "int") == 0) ||
                    (strcmp(lt, "string") == 0 && strcmp(rt, "string") == 0)) {
                    e->inferred_type = strdup("int");
                    return e->inferred_type;
                }
                if (symtab_find_class(st, lt) && symtab_find_class(st, rt)) {
                    if (symtab_is_subtype(st, lt, rt) || symtab_is_subtype(st, rt, lt)) {
                        e->inferred_type = strdup("int");
                        return e->inferred_type;
                    }
                }
                sem_error(13, e->line, "Equality operator requires compatible types");
            }
            sem_error(19, e->line, "Unknown binary operator");
            return NULL;
        }

        case EXPR_MEMBER: {
            Expr* obj = e->as.member.object;
            const char* mname = e->as.member.member;
            ClassInfo* target_cls = NULL;

            if (obj->kind == EXPR_ID && strcmp(obj->as.str_val, "super") == 0) {
                if (!cur_class || !cur_class->super_class) {
                    sem_error(14, e->line, "'super' used outside of a valid subclass");
                }
                target_cls = cur_class->super_class;
                obj->inferred_type = strdup(target_cls->name);
            } else {
                const char* obj_type = analyze_expr(st, cur_class, obj);
                target_cls = symtab_find_class(st, obj_type);
                if (!target_cls) {
                    sem_error(13, e->line, "Field access on non-object type");
                }
            }

            FieldInfo* fld = class_find_field(target_cls, mname);
            if (!fld) {
                sem_error(14, e->line, "Access to undefined field");
            }
            e->inferred_type = strdup(fld->type);
            return e->inferred_type;
        }

        case EXPR_CALL:
            return analyze_call(st, cur_class, e);
    }
    return NULL;
}

static void analyze_stmt_list(SymTable* st, ClassInfo* cur_class, const char* ret_type, StmtList* list) {
    for (size_t i = 0; i < list->count; i++) {
        Stmt* s = list->items[i];
        switch (s->kind) {
            case STMT_VAR_DECL: {
                const char* vtype = s->as.var_decl.type;
                if (!symtab_type_exists(st, vtype)) {
                    sem_error(14, s->line, "Variable declared with undefined type");
                }
                for (size_t j = 0; j < s->as.var_decl.names.count; j++) {
                    const char* vname = s->as.var_decl.names.items[j];
                    if (strcmp(vname, "this") == 0 || strcmp(vname, "super") == 0) {
                        sem_error(14, s->line, "Reserved identifier used as variable name");
                    }
                    if (symtab_lookup_var_current_scope(st, vname) != NULL) {
                        sem_error(14, s->line, "Redeclaration of variable in the same scope");
                    }
                    symtab_add_var(st, vname, vtype, 0);
                }
                break;
            }

            case STMT_ASSIGN: {
                Expr* lhs = s->as.assign.target;
                if (lhs->kind != EXPR_ID && lhs->kind != EXPR_MEMBER) {
                    sem_error(14, s->line, "Left-hand side of assignment is not an l-value");
                }
                if (lhs->kind == EXPR_ID &&
                    (strcmp(lhs->as.str_val, "this") == 0 || strcmp(lhs->as.str_val, "super") == 0)) {
                    sem_error(14, s->line, "Cannot assign to 'this' or 'super'");
                }
                const char* ltype = analyze_expr(st, cur_class, lhs);
                const char* rtype = analyze_expr(st, cur_class, s->as.assign.value);
                if (!is_assignable(st, ltype, rtype)) {
                    sem_error(13, s->line, "Incompatible types in assignment");
                }
                break;
            }

            case STMT_IF: {
                const char* ctype = analyze_expr(st, cur_class, s->as.if_stmt.cond);
                if (strcmp(ctype, "int") != 0) {
                    sem_error(13, s->line, "Condition in 'if' statement must be of type int");
                }
                symtab_push_scope(st);
                analyze_stmt_list(st, cur_class, ret_type, &s->as.if_stmt.then_block);
                symtab_pop_scope(st);

                symtab_push_scope(st);
                analyze_stmt_list(st, cur_class, ret_type, &s->as.if_stmt.else_block);
                symtab_pop_scope(st);
                break;
            }

            case STMT_WHILE: {
                const char* ctype = analyze_expr(st, cur_class, s->as.while_stmt.cond);
                if (strcmp(ctype, "int") != 0) {
                    sem_error(13, s->line, "Condition in 'while' statement must be of type int");
                }
                symtab_push_scope(st);
                analyze_stmt_list(st, cur_class, ret_type, &s->as.while_stmt.body);
                symtab_pop_scope(st);
                break;
            }

            case STMT_RETURN: {
                if (strcmp(ret_type, "void") == 0) {
                    if (s->as.ret_stmt.value != NULL) {
                        sem_error(13, s->line, "Void function/method cannot return a value");
                    }
                } else {
                    if (s->as.ret_stmt.value == NULL) {
                        sem_error(13, s->line, "Non-void function/method must return a value");
                    }
                    const char* vtype = analyze_expr(st, cur_class, s->as.ret_stmt.value);
                    if (!is_assignable(st, ret_type, vtype)) {
                        sem_error(13, s->line, "Return value type does not match declared return type");
                    }
                }
                break;
            }

            case STMT_EXPR:
                analyze_expr(st, cur_class, s->as.expr_stmt.expr);
                break;
        }
    }
}

static void analyze_func_body(SymTable* st, ClassInfo* cur_class, FuncDecl* fdecl, FuncInfo* finfo) {
    symtab_push_scope(st);
    st->next_local_offset = 1; /* Variables locales : offsets +1, +2, ... relatifs à $FP */

    /* Convention d'appel :
     * Si méthode : this est à FP - 1, param_0 à FP - 2, ..., param_{n-1} à FP - (n + 1)
     * Si fonction : param_0 à FP - 1, ..., param_{n-1} à FP - n
     */
    int param_start_offset = cur_class ? -2 : -1;
    if (cur_class) {
        symtab_add_var(st, "this", cur_class->name, -1);
    }

    for (size_t i = 0; i < fdecl->params.count; i++) {
        const char* pname = fdecl->params.items[i].name;
        const char* ptype = fdecl->params.items[i].type;
        if (strcmp(pname, "this") == 0 || strcmp(pname, "super") == 0) {
            sem_error(14, fdecl->line, "Reserved identifier used as parameter name");
        }
        if (symtab_lookup_var_current_scope(st, pname) != NULL) {
            sem_error(14, fdecl->line, "Duplicate parameter name");
        }
        symtab_add_var(st, pname, ptype, param_start_offset - (int)i);
    }

    analyze_stmt_list(st, cur_class, fdecl->return_type, &fdecl->body);
    finfo->local_count = st->next_local_offset - 1;
    symtab_pop_scope(st);
}

SymTable* semantic_analyze(Program* prog) {
    SymTable* st = symtab_new();

    /* Passe 1A : Enregistrer toutes les classes */
    for (size_t i = 0; i < prog->class_count; i++) {
        ClassDecl* c = prog->classes[i];
        if (symtab_find_class(st, c->name) != NULL ||
            strcmp(c->name, "int") == 0 || strcmp(c->name, "string") == 0 || strcmp(c->name, "void") == 0) {
            sem_error(14, c->line, "Duplicate or reserved class name");
        }
        symtab_add_class(st, c->name, c->super_name, c);
    }

    /* Passe 1B : Enregistrer toutes les fonctions globales */
    for (size_t i = 0; i < prog->func_count; i++) {
        FuncDecl* f = prog->functions[i];
        if (symtab_find_func(st, f->name) != NULL ||
            strcmp(f->name, "this") == 0 || strcmp(f->name, "super") == 0) {
            sem_error(14, f->line, "Duplicate or reserved function name");
        }
        if (strcmp(f->return_type, "void") != 0 && !symtab_type_exists(st, f->return_type)) {
            sem_error(14, f->line, "Unknown function return type");
        }
        for (size_t p = 0; p < f->params.count; p++) {
            if (!symtab_type_exists(st, f->params.items[p].type)) {
                sem_error(14, f->line, "Unknown function parameter type");
            }
        }
        symtab_add_func(st, f->name, f->return_type, f->params, false);
    }

    /* Vérification obligatoire de void main(void) */
    FuncInfo* main_fn = symtab_find_func(st, "main");
    if (!main_fn || strcmp(main_fn->return_type, "void") != 0 || main_fn->params.count != 0) {
        sem_error(14, 1, "Program must define 'void main(void)'");
    }

    /* Passe 1C : Résoudre l'héritage, les offsets d'attributs et les Method Tables */
    for (size_t i = 1; i < st->class_count; i++) { /* 0 est Object */
        resolve_class_hierarchy(st, st->classes[i]);
    }

    /* Passe 2 : Analyser les corps des fonctions globales et des méthodes */
    for (size_t i = 0; i < prog->func_count; i++) {
        FuncDecl* fdecl = prog->functions[i];
        FuncInfo* finfo = symtab_find_func(st, fdecl->name);
        analyze_func_body(st, NULL, fdecl, finfo);
    }

    for (size_t i = 1; i < st->class_count; i++) {
        ClassInfo* cls = st->classes[i];
        ClassDecl* cdecl = cls->ast_node;
        for (size_t m = 0; m < cdecl->method_count; m++) {
            FuncDecl* mdecl = cdecl->methods[m];
            FuncInfo* minfo = class_find_method(cls, mdecl->name);
            analyze_func_body(st, cls, mdecl, minfo);
        }
    }

    return st;
}