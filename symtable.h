/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "ast.h"
#include <stdbool.h>

typedef struct ClassInfo ClassInfo;
typedef struct FuncInfo FuncInfo;

typedef struct {
    char* name;
    char* type;
    int offset; /* Index dans le chunk de l'objet (>= 1, 0 = pointeur MT) */
} FieldInfo;

struct FuncInfo {
    char* name;
    char* return_type;
    ParamList params;
    bool is_builtin;
    bool is_method;
    char* owner_class; /* Nom de la classe qui définit effectivement cette implémentation */
    int mt_index;      /* Index dans le chunk Method Table (si méthode) */
    int local_count;   /* Nombre total de variables locales (pour la frame pile) */
};

struct ClassInfo {
    char* name;
    char* super_name;
    ClassInfo* super_class;
    ClassDecl* ast_node;
    bool hierarchy_resolved;
    bool visiting; /* Détection de cycles d'héritage */

    FieldInfo* fields;
    size_t field_count;
    size_t field_capacity;

    FuncInfo** methods; /* Table ordonnée des méthodes (vtable) */
    size_t method_count;
    size_t method_capacity;
};

typedef struct {
    char* name;
    char* type;
    int fp_offset; /* Offset relatif à $FP (négatif pour params/this, positif pour locales) */
} VarSymbol;

typedef struct Scope {
    VarSymbol* vars;
    size_t count;
    size_t capacity;
    struct Scope* parent;
} Scope;

typedef struct {
    ClassInfo** classes;
    size_t class_count;
    size_t class_capacity;

    FuncInfo** functions;
    size_t func_count;
    size_t func_capacity;

    Scope* current_scope;
    int next_local_offset;
} SymTable;

SymTable* symtab_new(void);
bool symtab_type_exists(SymTable* st, const char* type_name);
bool symtab_is_subtype(SymTable* st, const char* sub, const char* sup);

ClassInfo* symtab_add_class(SymTable* st, const char* name, const char* super_name, ClassDecl* node);
ClassInfo* symtab_find_class(SymTable* st, const char* name);

FuncInfo* symtab_add_func(SymTable* st, const char* name, const char* ret_type, ParamList params, bool is_builtin);
FuncInfo* symtab_find_func(SymTable* st, const char* name);

FieldInfo* class_find_field(ClassInfo* cls, const char* name);
FuncInfo* class_find_method(ClassInfo* cls, const char* name);
void class_add_field(ClassInfo* cls, const char* name, const char* type);
void class_set_or_add_method(ClassInfo* cls, FuncInfo* method);

void symtab_push_scope(SymTable* st);
void symtab_pop_scope(SymTable* st);
VarSymbol* symtab_add_var(SymTable* st, const char* name, const char* type, int custom_offset);
VarSymbol* symtab_lookup_var(SymTable* st, const char* name);
VarSymbol* symtab_lookup_var_current_scope(SymTable* st, const char* name);

#endif