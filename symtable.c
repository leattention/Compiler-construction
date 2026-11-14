/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void* xmalloc(size_t s) {
    void* p = malloc(s);
    if (!p) { fprintf(stderr, "Internal error: OOM\n"); exit(19); }
    return p;
}

static void* xrealloc(void* ptr, size_t s) {
    void* p = realloc(ptr, s);
    if (!p) { fprintf(stderr, "Internal error: OOM\n"); exit(19); }
    return p;
}

SymTable* symtab_new(void) {
    SymTable* st = xmalloc(sizeof(SymTable));
    st->classes = NULL; st->class_count = 0; st->class_capacity = 0;
    st->functions = NULL; st->func_count = 0; st->func_capacity = 0;
    st->current_scope = NULL;
    st->next_local_offset = 1;

    /* 1. Classe racine intégrée : Object */
    ClassInfo* obj = symtab_add_class(st, "Object", NULL, NULL);
    obj->hierarchy_resolved = true;

    FuncInfo* to_str = xmalloc(sizeof(FuncInfo));
    *to_str = (FuncInfo){strdup("toString"), strdup("string"), {NULL, 0, 0}, true, true, strdup("Object"), 0, 0};
    class_set_or_add_method(obj, to_str);

    FuncInfo* get_cls = xmalloc(sizeof(FuncInfo));
    *get_cls = (FuncInfo){strdup("getClass"), strdup("string"), {NULL, 0, 0}, true, true, strdup("Object"), 1, 0};
    class_set_or_add_method(obj, get_cls);

    /* 2. Fonctions globales intégrées */
    ParamList p_empty = {NULL, 0, 0};
    symtab_add_func(st, "readInt", "int", p_empty, true);
    symtab_add_func(st, "readString", "string", p_empty, true);

    ParamList p_len = {NULL, 0, 0};
    param_list_push(&p_len, strdup("string"), strdup("s"));
    symtab_add_func(st, "length", "int", p_len, true);

    ParamList p_sub = {NULL, 0, 0};
    param_list_push(&p_sub, strdup("string"), strdup("s"));
    param_list_push(&p_sub, strdup("int"), strdup("i"));
    param_list_push(&p_sub, strdup("int"), strdup("n"));
    symtab_add_func(st, "subStr", "string", p_sub, true);

    /* print est variadique (au moins 1 param int ou string), géré spécialement en sémantique */
    symtab_add_func(st, "print", "void", p_empty, true);

    return st;
}

bool symtab_type_exists(SymTable* st, const char* type_name) {
    if (!type_name) return false;
    if (strcmp(type_name, "int") == 0 || strcmp(type_name, "string") == 0) return true;
    return symtab_find_class(st, type_name) != NULL;
}

bool symtab_is_subtype(SymTable* st, const char* sub, const char* sup) {
    if (!sub || !sup) return false;
    if (strcmp(sub, sup) == 0) return true;
    ClassInfo* c = symtab_find_class(st, sub);
    while (c && c->super_class) {
        c = c->super_class;
        if (strcmp(c->name, sup) == 0) return true;
    }
    return false;
}

ClassInfo* symtab_add_class(SymTable* st, const char* name, const char* super_name, ClassDecl* node) {
    if (st->class_count == st->class_capacity) {
        st->class_capacity = st->class_capacity ? st->class_capacity * 2 : 8;
        st->classes = xrealloc(st->classes, st->class_capacity * sizeof(ClassInfo*));
    }
    ClassInfo* c = xmalloc(sizeof(ClassInfo));
    c->name = strdup(name);
    c->super_name = super_name ? strdup(super_name) : NULL;
    c->super_class = NULL;
    c->ast_node = node;
    c->hierarchy_resolved = false;
    c->visiting = false;
    c->fields = NULL; c->field_count = 0; c->field_capacity = 0;
    c->methods = NULL; c->method_count = 0; c->method_capacity = 0;
    st->classes[st->class_count++] = c;
    return c;
}

ClassInfo* symtab_find_class(SymTable* st, const char* name) {
    if (!name) return NULL;
    for (size_t i = 0; i < st->class_count; i++) {
        if (strcmp(st->classes[i]->name, name) == 0) return st->classes[i];
    }
    return NULL;
}

FuncInfo* symtab_add_func(SymTable* st, const char* name, const char* ret_type, ParamList params, bool is_builtin) {
    if (st->func_count == st->func_capacity) {
        st->func_capacity = st->func_capacity ? st->func_capacity * 2 : 8;
        st->functions = xrealloc(st->functions, st->func_capacity * sizeof(FuncInfo*));
    }
    FuncInfo* f = xmalloc(sizeof(FuncInfo));
    f->name = strdup(name);
    f->return_type = strdup(ret_type);
    f->params = params;
    f->is_builtin = is_builtin;
    f->is_method = false;
    f->owner_class = NULL;
    f->mt_index = -1;
    f->local_count = 0;
    st->functions[st->func_count++] = f;
    return f;
}

FuncInfo* symtab_find_func(SymTable* st, const char* name) {
    if (!name) return NULL;
    for (size_t i = 0; i < st->func_count; i++) {
        if (strcmp(st->functions[i]->name, name) == 0) return st->functions[i];
    }
    return NULL;
}

FieldInfo* class_find_field(ClassInfo* cls, const char* name) {
    for (size_t i = 0; i < cls->field_count; i++) {
        if (strcmp(cls->fields[i].name, name) == 0) return &cls->fields[i];
    }
    return NULL;
}

FuncInfo* class_find_method(ClassInfo* cls, const char* name) {
    for (size_t i = 0; i < cls->method_count; i++) {
        if (strcmp(cls->methods[i]->name, name) == 0) return cls->methods[i];
    }
    return NULL;
}

void class_add_field(ClassInfo* cls, const char* name, const char* type) {
    if (cls->field_count == cls->field_capacity) {
        cls->field_capacity = cls->field_capacity ? cls->field_capacity * 2 : 8;
        cls->fields = xrealloc(cls->fields, cls->field_capacity * sizeof(FieldInfo));
    }
    /* Offset = field_count + 1 car le mot 0 de l'objet est l'ID de la Method Table */
    int offset = (int)cls->field_count + 1;
    cls->fields[cls->field_count++] = (FieldInfo){strdup(name), strdup(type), offset};
}

void class_set_or_add_method(ClassInfo* cls, FuncInfo* method) {
    for (size_t i = 0; i < cls->method_count; i++) {
        if (strcmp(cls->methods[i]->name, method->name) == 0) {
            method->mt_index = (int)i;
            cls->methods[i] = method;
            return;
        }
    }
    if (cls->method_count == cls->method_capacity) {
        cls->method_capacity = cls->method_capacity ? cls->method_capacity * 2 : 8;
        cls->methods = xrealloc(cls->methods, cls->method_capacity * sizeof(FuncInfo*));
    }
    method->mt_index = (int)cls->method_count;
    cls->methods[cls->method_count++] = method;
}

void symtab_push_scope(SymTable* st) {
    Scope* s = xmalloc(sizeof(Scope));
    s->vars = NULL; s->count = 0; s->capacity = 0;
    s->parent = st->current_scope;
    st->current_scope = s;
}

void symtab_pop_scope(SymTable* st) {
    if (!st->current_scope) return;
    Scope* old = st->current_scope;
    st->current_scope = old->parent;
    free(old->vars);
    free(old);
}

VarSymbol* symtab_add_var(SymTable* st, const char* name, const char* type, int custom_offset) {
    Scope* s = st->current_scope;
    if (s->count == s->capacity) {
        s->capacity = s->capacity ? s->capacity * 2 : 8;
        s->vars = xrealloc(s->vars, s->capacity * sizeof(VarSymbol));
    }
    int off = (custom_offset != 0) ? custom_offset : st->next_local_offset++;
    s->vars[s->count] = (VarSymbol){strdup(name), strdup(type), off};
    return &s->vars[s->count++];
}

VarSymbol* symtab_lookup_var(SymTable* st, const char* name) {
    for (Scope* s = st->current_scope; s != NULL; s = s->parent) {
        for (size_t i = 0; i < s->count; i++) {
            if (strcmp(s->vars[i].name, name) == 0) return &s->vars[i];
        }
    }
    return NULL;
}

VarSymbol* symtab_lookup_var_current_scope(SymTable* st, const char* name) {
    if (!st->current_scope) return NULL;
    for (size_t i = 0; i < st->current_scope->count; i++) {
        if (strcmp(st->current_scope->vars[i].name, name) == 0) {
            return &st->current_scope->vars[i];
        }
    }
    return NULL;
}