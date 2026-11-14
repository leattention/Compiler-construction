/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    FILE* out;
    SymTable* st;
    ClassInfo* cur_class;
    FuncInfo* cur_func;
    int label_counter;
} CodeGen;

static void cg_error(const char* msg) {
    fprintf(stderr, "Code generation error: %s\n", msg);
    exit(15);
}

static int next_label(CodeGen* cg) {
    return cg->label_counter++;
}

static int get_class_index(CodeGen* cg, const char* cname) {
    for (size_t i = 0; i < cg->st->class_count; i++) {
        if (strcmp(cg->st->classes[i]->name, cname) == 0) return (int)i;
    }
    return 0;
}

/* Ajuste l'offset de symtable.c pour intégrer l'adresse de retour en [$0-1] */
static int actual_fp_offset(int sym_offset) {
    return (sym_offset < 0) ? (sym_offset - 1) : sym_offset;
}

static void push_reg(CodeGen* cg, const char* reg) {
    fprintf(cg->out, "\tSET [$SP+1], %s\n", reg);
    fprintf(cg->out, "\tADDI $SP, $SP, 1\n");
}

static void pop_reg(CodeGen* cg, const char* reg) {
    fprintf(cg->out, "\tSET %s, [$SP]\n", reg);
    fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
}

/* Chaîne = chunk de N mots, chaque mot étant une chaîne de 1 caractère ("a") */
static void emit_empty_string(CodeGen* cg, const char* reg) {
    fprintf(cg->out, "\tCREATE %s, 0\n", reg);
}

/* Décode une chaîne littérale échappée du scanner et alloue le chunk caractère par caractère */
static void emit_string_literal(CodeGen* cg, const char* reg, const char* lit) {
    char* chars[4096];
    size_t count = 0;
    const char* p = lit;

    while (*p && count < 4096) {
        char buf[16];
        if (*p == '\\' && *(p + 1) != '\0') {
            p++;
            if (*p == 'n') {
                strcpy(buf, "\\n");
                p++;
            } else if (*p == 't') {
                strcpy(buf, "\\t");
                p++;
            } else if (*p == '\\') {
                strcpy(buf, "\\\\");
                p++;
            } else if (*p == '"') {
                strcpy(buf, "\\\"");
                p++;
            } else if (*p == 'x') {
                char hex[7] = {0};
                strncpy(hex, p + 1, 6);
                unsigned long cp = strtoul(hex, NULL, 16);
                if (cp < 0x80) {
                    if (cp == '"') strcpy(buf, "\\\"");
                    else if (cp == '\\') strcpy(buf, "\\\\");
                    else if (cp == '\n') strcpy(buf, "\\n");
                    else if (cp == '\t') strcpy(buf, "\\t");
                    else { buf[0] = (char)cp; buf[1] = '\0'; }
                } else if (cp < 0x800) {
                    buf[0] = (char)(0xC0 | (cp >> 6));
                    buf[1] = (char)(0x80 | (cp & 0x3F));
                    buf[2] = '\0';
                } else if (cp < 0x10000) {
                    buf[0] = (char)(0xE0 | (cp >> 12));
                    buf[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
                    buf[2] = (char)(0x80 | (cp & 0x3F));
                    buf[3] = '\0';
                } else {
                    buf[0] = (char)(0xF0 | (cp >> 18));
                    buf[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
                    buf[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
                    buf[3] = (char)(0x80 | (cp & 0x3F));
                    buf[4] = '\0';
                }
                p += 7;
            } else {
                buf[0] = *p++;
                buf[1] = '\0';
            }
        } else {
            unsigned char c = (unsigned char)*p;
            size_t clen = 1;
            if ((c & 0xE0) == 0xC0) clen = 2;
            else if ((c & 0xF0) == 0xE0) clen = 3;
            else if ((c & 0xF8) == 0xF0) clen = 4;
            memcpy(buf, p, clen);
            buf[clen] = '\0';
            p += clen;
        }
        chars[count++] = strdup(buf);
    }

    fprintf(cg->out, "\tCREATE %s, %zu\n", reg, count);
    for (size_t i = 0; i < count; i++) {
        fprintf(cg->out, "\tSETWORD %s, %zu, \"%s\"\n", reg, i, chars[i]);
        free(chars[i]);
    }
}

/* Copie profonde d'une chaîne (chunk) */
static void emit_string_copy(CodeGen* cg, const char* dst_reg, const char* src_reg) {
    int lbl_ok = next_label(cg);
    int lbl_end = next_label(cg);
    fprintf(cg->out, "\tJUMPNZ L_str_ok_%d, %s\n", lbl_ok, src_reg);
    emit_empty_string(cg, dst_reg);
    fprintf(cg->out, "\tJUMP L_str_end_%d\n", lbl_end);
    fprintf(cg->out, "LABEL L_str_ok_%d\n", lbl_ok);
    fprintf(cg->out, "\tCOPY %s, %s\n", dst_reg, src_reg);
    fprintf(cg->out, "LABEL L_str_end_%d\n", lbl_end);
}

static void gen_expr(CodeGen* cg, Expr* e);
static void gen_stmt_list(CodeGen* cg, StmtList* list);

/* Convention registres : $0 = Frame Pointer (FP), $1 = Valeur de retour (RET) */
static void emit_prologue(CodeGen* cg, const char* label, int local_count) {
    fprintf(cg->out, "\nLABEL %s\n", label);
    fprintf(cg->out, "\tADDI $SP, $SP, 1\n");
    push_reg(cg, "$0");
    fprintf(cg->out, "\tSET $0, $SP\n");
    if (local_count > 0) {
        fprintf(cg->out, "\tADDI $SP, $SP, %d\n", local_count);
    }
}

static void emit_epilogue(CodeGen* cg) {
    fprintf(cg->out, "\tSET $SP, $0\n");
    pop_reg(cg, "$0");
    fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
    fprintf(cg->out, "\tRETURN [$SP+1]\n");
}

/* Enregistre et initialise les variables locales d'un bloc à 0 ou "" */
static void init_local_vars(CodeGen* cg, StmtList* list) {
    for (size_t i = 0; i < list->count; i++) {
        Stmt* s = list->items[i];
        if (s->kind == STMT_VAR_DECL) {
            const char* vtype = s->as.var_decl.type;
            bool is_str = (strcmp(vtype, "string") == 0);
            for (size_t j = 0; j < s->as.var_decl.names.count; j++) {
                const char* vname = s->as.var_decl.names.items[j];
                VarSymbol* vs = symtab_add_var(cg->st, vname, vtype, cg->st->next_local_offset++);
                int off = actual_fp_offset(vs->fp_offset);
                if (is_str) {
                    emit_empty_string(cg, "$2");
                    fprintf(cg->out, "\tSET [$0+%d], $2\n", off);
                } else {
                    fprintf(cg->out, "\tSET [$0+%d], 0\n", off);
                }
            }
        }
    }
}

/* Génère le code d'un appel à une fonction intégrée (print, readInt, readString, length, subStr) */
static bool gen_builtin_func_call(CodeGen* cg, const char* fname, ExprList* args) {
    if (strcmp(fname, "print") == 0) {
        for (size_t i = 0; i < args->count; i++) {
            Expr* arg = args->items[i];
            gen_expr(cg, arg);
            pop_reg(cg, "$2");
            if (strcmp(arg->inferred_type, "int") == 0) {
                fprintf(cg->out, "\tWRITEI $2\n");
            } else {
                push_reg(cg, "$2");
                fprintf(cg->out, "\tCALL [$SP+1], L_rt_print_str\n");
                fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
            }
        }
        return true;
    }
    if (strcmp(fname, "readInt") == 0) {
        fprintf(cg->out, "\tREADI $1\n");
        push_reg(cg, "$1");
        return true;
    }
    if (strcmp(fname, "readString") == 0) {
        fprintf(cg->out, "\tCALL [$SP+1], L_rt_read_str\n");
        push_reg(cg, "$1");
        return true;
    }
    if (strcmp(fname, "length") == 0) {
        gen_expr(cg, args->items[0]);
        pop_reg(cg, "$2");
        fprintf(cg->out, "\tGETSIZE $1, $2\n");
        push_reg(cg, "$1");
        return true;
    }
    if (strcmp(fname, "subStr") == 0) {
        for (int i = (int)args->count - 1; i >= 0; i--) {
            gen_expr(cg, args->items[i]);
        }
        fprintf(cg->out, "\tCALL [$SP+1], L_builtin_substr\n");
        fprintf(cg->out, "\tSUBI $SP, $SP, %zu\n", args->count);
        push_reg(cg, "$1");
        return true;
    }
    return false;
}

/* Évalue une expression et empile son résultat au sommet de $SP */
static void gen_expr(CodeGen* cg, Expr* e) {
    switch (e->kind) {
        case EXPR_INT_LIT:
            fprintf(cg->out, "\tSET $2, %lld\n", (long long)e->as.int_val);
            push_reg(cg, "$2");
            break;

        case EXPR_STR_LIT:
            emit_string_literal(cg, "$2", e->as.str_val);
            push_reg(cg, "$2");
            break;

        case EXPR_ID: {
            const char* name = e->as.str_val;
            if (strcmp(name, "this") == 0) {
                fprintf(cg->out, "\tSET $2, [$0-2]\n");
                push_reg(cg, "$2");
                break;
            }
            VarSymbol* vs = symtab_lookup_var(cg->st, name);
            if (vs) {
                int off = actual_fp_offset(vs->fp_offset);
                if (off >= 0) fprintf(cg->out, "\tSET $2, [$0+%d]\n", off);
                else          fprintf(cg->out, "\tSET $2, [$0%d]\n", off);
                push_reg(cg, "$2");
                break;
            }
            if (cg->cur_class) {
                FieldInfo* fld = class_find_field(cg->cur_class, name);
                if (fld) {
                    fprintf(cg->out, "\tSET $2, [$0-2]\n");
                    fprintf(cg->out, "\tGETWORD $3, $2, %d\n", fld->offset);
                    push_reg(cg, "$3");
                    break;
                }
            }
            cg_error("Unresolved identifier in codegen");
            break;
        }

        case EXPR_NEW: {
            const char* cname = e->as.str_val;
            ClassInfo* cls = symtab_find_class(cg->st, cname);
            if (!cls) cg_error("Unknown class in new");

            fprintf(cg->out, "\tCREATE $2, %zu\n", cls->field_count + 1);
            int cidx = get_class_index(cg, cls->name);
            fprintf(cg->out, "\tSET $3, [0]\n");
            fprintf(cg->out, "\tGETWORD $3, $3, %d\n", cidx);
            fprintf(cg->out, "\tSETWORD $2, 0, $3\n");

            for (size_t i = 0; i < cls->field_count; i++) {
                if (strcmp(cls->fields[i].type, "string") == 0) {
                    emit_empty_string(cg, "$3");
                    fprintf(cg->out, "\tSETWORD $2, %d, $3\n", cls->fields[i].offset);
                } else {
                    fprintf(cg->out, "\tSETWORD $2, %d, 0\n", cls->fields[i].offset);
                }
            }

            ClassInfo* chain[64];
            int depth = 0;
            for (ClassInfo* c = cls; c != NULL && strcmp(c->name, "Object") != 0; c = c->super_class) {
                chain[depth++] = c;
            }
            for (int i = depth - 1; i >= 0; i--) {
                FuncInfo* ctor = class_find_method(chain[i], chain[i]->name);
                if (ctor && strcmp(ctor->owner_class, chain[i]->name) == 0) {
                    push_reg(cg, "$2");
                    push_reg(cg, "$2");
                    fprintf(cg->out, "\tCALL [$SP+1], L_m_%s_%s\n", chain[i]->name, chain[i]->name);
                    fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
                    pop_reg(cg, "$2");
                }
            }

            push_reg(cg, "$2");
            break;
        }

        case EXPR_CAST: {
            gen_expr(cg, e->as.cast.expr);
            const char* src = e->as.cast.expr->inferred_type;
            const char* dst = e->as.cast.target_type;

            if (strcmp(dst, "string") == 0 && strcmp(src, "int") == 0) {
                fprintf(cg->out, "\tCALL [$SP+1], L_rt_int2str\n");
                fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
                push_reg(cg, "$1");
                break;
            }

            pop_reg(cg, "$2");
            int lbl_ok = next_label(cg);
            int lbl_loop = next_label(cg);
            int lbl_fail = next_label(cg);
            int target_cidx = get_class_index(cg, dst);

            fprintf(cg->out, "\tJUMPZ L_cast_ok_%d, $2\n", lbl_ok);
            fprintf(cg->out, "\tGETWORD $3, $2, 0\n");
            fprintf(cg->out, "\tSET $5, [0]\n");
            fprintf(cg->out, "\tGETWORD $5, $5, %d\n", target_cidx);
            fprintf(cg->out, "LABEL L_cast_loop_%d\n", lbl_loop);
            fprintf(cg->out, "\tEQI $4, $3, $5\n");
            fprintf(cg->out, "\tJUMPNZ L_cast_ok_%d, $4\n", lbl_ok);
            fprintf(cg->out, "\tGETWORD $3, $3, 0\n");
            fprintf(cg->out, "\tJUMPNZ L_cast_loop_%d, $3\n", lbl_loop);

            fprintf(cg->out, "LABEL L_cast_fail_%d\n", lbl_fail);
            fprintf(cg->out, "\tGETWORD $2, 0, 0\n");

            fprintf(cg->out, "LABEL L_cast_ok_%d\n", lbl_ok);
            push_reg(cg, "$2");
            break;
        }

        case EXPR_UNARY: {
            gen_expr(cg, e->as.unary.operand);
            pop_reg(cg, "$2");
            fprintf(cg->out, "\tNOT $2, $2\n");
            push_reg(cg, "$2");
            break;
        }

        case EXPR_BINARY: {
            const char* op = e->as.binary.op;
            const char* lt = e->as.binary.left->inferred_type;

            gen_expr(cg, e->as.binary.left);
            gen_expr(cg, e->as.binary.right);

            if (strcmp(op, "+") == 0) {
                if (strcmp(lt, "string") == 0) {
                    fprintf(cg->out, "\tCALL [$SP+1], L_rt_concat\n");
                    fprintf(cg->out, "\tSUBI $SP, $SP, 2\n");
                    push_reg(cg, "$1");
                } else {
                    pop_reg(cg, "$3");
                    pop_reg(cg, "$2");
                    fprintf(cg->out, "\tADDI $2, $2, $3\n");
                    push_reg(cg, "$2");
                }
            } else if (strcmp(op, "-") == 0) {
                pop_reg(cg, "$3");
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tSUBI $2, $2, $3\n");
                push_reg(cg, "$2");
            } else if (strcmp(op, "*") == 0) {
                pop_reg(cg, "$3");
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tMULI $2, $2, $3\n");
                push_reg(cg, "$2");
            } else if (strcmp(op, "/") == 0) {
                pop_reg(cg, "$3");
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tDIVI $2, $2, $3\n");
                push_reg(cg, "$2");
            } else if (strcmp(op, "&&") == 0) {
                pop_reg(cg, "$3");
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tAND $2, $2, $3\n");
                push_reg(cg, "$2");
            } else if (strcmp(op, "||") == 0) {
                pop_reg(cg, "$3");
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tOR $2, $2, $3\n");
                push_reg(cg, "$2");
            } else if (strcmp(op, "<") == 0 || strcmp(op, "<=") == 0 ||
                       strcmp(op, ">") == 0 || strcmp(op, ">=") == 0 ||
                       strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
                if (strcmp(lt, "string") == 0) {
                    fprintf(cg->out, "\tCALL [$SP+1], L_rt_strcmp\n");
                    fprintf(cg->out, "\tSUBI $SP, $SP, 2\n");
                    if (strcmp(op, "<") == 0)       fprintf(cg->out, "\tLTI $4, $1, 0\n");
                    else if (strcmp(op, ">") == 0)  fprintf(cg->out, "\tGTI $4, $1, 0\n");
                    else if (strcmp(op, "==") == 0) fprintf(cg->out, "\tEQI $4, $1, 0\n");
                    else if (strcmp(op, "!=") == 0) { fprintf(cg->out, "\tEQI $4, $1, 0\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    else if (strcmp(op, "<=") == 0) { fprintf(cg->out, "\tGTI $4, $1, 0\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    else if (strcmp(op, ">=") == 0) { fprintf(cg->out, "\tLTI $4, $1, 0\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    push_reg(cg, "$4");
                } else {
                    pop_reg(cg, "$3");
                    pop_reg(cg, "$2");
                    if (strcmp(op, "<") == 0)       fprintf(cg->out, "\tLTI $4, $2, $3\n");
                    else if (strcmp(op, ">") == 0)  fprintf(cg->out, "\tGTI $4, $2, $3\n");
                    else if (strcmp(op, "==") == 0) fprintf(cg->out, "\tEQI $4, $2, $3\n");
                    else if (strcmp(op, "!=") == 0) { fprintf(cg->out, "\tEQI $4, $2, $3\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    else if (strcmp(op, "<=") == 0) { fprintf(cg->out, "\tGTI $4, $2, $3\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    else if (strcmp(op, ">=") == 0) { fprintf(cg->out, "\tLTI $4, $2, $3\n"); fprintf(cg->out, "\tNOT $4, $4\n"); }
                    push_reg(cg, "$4");
                }
            }
            break;
        }

        case EXPR_MEMBER: {
            Expr* obj = e->as.member.object;
            const char* mname = e->as.member.member;
            ClassInfo* cls = NULL;

            if (obj->kind == EXPR_ID && strcmp(obj->as.str_val, "super") == 0) {
                cls = cg->cur_class->super_class;
                fprintf(cg->out, "\tSET $2, [$0-2]\n");
            } else {
                gen_expr(cg, obj);
                pop_reg(cg, "$2");
                cls = symtab_find_class(cg->st, obj->inferred_type);
            }

            FieldInfo* fld = class_find_field(cls, mname);
            fprintf(cg->out, "\tGETWORD $3, $2, %d\n", fld->offset);
            push_reg(cg, "$3");
            break;
        }

        case EXPR_CALL: {
            Expr* callee = e->as.call.callee;
            ExprList* args = &e->as.call.args;

            if (callee->kind == EXPR_ID) {
                const char* fname = callee->as.str_val;
                if (gen_builtin_func_call(cg, fname, args)) break;

                FuncInfo* fn = symtab_find_func(cg->st, fname);
                for (int i = (int)args->count - 1; i >= 0; i--) {
                    gen_expr(cg, args->items[i]);
                }
                fprintf(cg->out, "\tCALL [$SP+1], L_fn_%s\n", fn->name);
                if (args->count > 0) {
                    fprintf(cg->out, "\tSUBI $SP, $SP, %zu\n", args->count);
                }
                if (strcmp(fn->return_type, "void") != 0) {
                    push_reg(cg, "$1");
                }
                break;
            }

            if (callee->kind == EXPR_MEMBER) {
                Expr* obj = callee->as.member.object;
                const char* mname = callee->as.member.member;
                bool is_super = (obj->kind == EXPR_ID && strcmp(obj->as.str_val, "super") == 0);
                ClassInfo* cls = is_super ? cg->cur_class->super_class
                                          : symtab_find_class(cg->st, obj->inferred_type);
                FuncInfo* method = class_find_method(cls, mname);

                for (int i = (int)args->count - 1; i >= 0; i--) {
                    gen_expr(cg, args->items[i]);
                }

                if (is_super) {
                    fprintf(cg->out, "\tSET $2, [$0-2]\n");
                    push_reg(cg, "$2");
                    fprintf(cg->out, "\tCALL [$SP+1], L_m_%s_%s\n", method->owner_class, method->name);
                } else {
                    gen_expr(cg, obj);
                    fprintf(cg->out, "\tSET $2, [$SP]\n");
                    fprintf(cg->out, "\tGETWORD $3, $2, 0\n");
                    fprintf(cg->out, "\tGETWORD $4, $3, %d\n", 2 + method->mt_index);
                    fprintf(cg->out, "\tCALL [$SP+1], $4\n");
                }

                fprintf(cg->out, "\tSUBI $SP, $SP, %zu\n", args->count + 1);
                if (strcmp(method->return_type, "void") != 0) {
                    push_reg(cg, "$1");
                }
                break;
            }
            cg_error("Invalid call target in codegen");
            break;
        }
    }
}

static void gen_stmt_list(CodeGen* cg, StmtList* list) {
    init_local_vars(cg, list);

    for (size_t i = 0; i < list->count; i++) {
        Stmt* s = list->items[i];
        switch (s->kind) {
            case STMT_VAR_DECL:
                break;

            case STMT_ASSIGN: {
                Expr* lhs = s->as.assign.target;
                bool is_str = (strcmp(lhs->inferred_type, "string") == 0);

                gen_expr(cg, s->as.assign.value);
                pop_reg(cg, "$2");
                if (is_str) {
                    emit_string_copy(cg, "$3", "$2");
                    fprintf(cg->out, "\tSET $2, $3\n");
                }

                if (lhs->kind == EXPR_ID) {
                    const char* name = lhs->as.str_val;
                    VarSymbol* vs = symtab_lookup_var(cg->st, name);
                    if (vs) {
                        int off = actual_fp_offset(vs->fp_offset);
                        if (off >= 0) fprintf(cg->out, "\tSET [$0+%d], $2\n", off);
                        else          fprintf(cg->out, "\tSET [$0%d], $2\n", off);
                    } else if (cg->cur_class) {
                        FieldInfo* fld = class_find_field(cg->cur_class, name);
                        fprintf(cg->out, "\tSET $4, [$0-2]\n");
                        fprintf(cg->out, "\tSETWORD $4, %d, $2\n", fld->offset);
                    }
                } else if (lhs->kind == EXPR_MEMBER) {
                    push_reg(cg, "$2");
                    Expr* obj = lhs->as.member.object;
                    ClassInfo* cls = NULL;
                    if (obj->kind == EXPR_ID && strcmp(obj->as.str_val, "super") == 0) {
                        cls = cg->cur_class->super_class;
                        fprintf(cg->out, "\tSET $4, [$0-2]\n");
                    } else {
                        gen_expr(cg, obj);
                        pop_reg(cg, "$4");
                        cls = symtab_find_class(cg->st, obj->inferred_type);
                    }
                    pop_reg(cg, "$2");
                    FieldInfo* fld = class_find_field(cls, lhs->as.member.member);
                    fprintf(cg->out, "\tSETWORD $4, %d, $2\n", fld->offset);
                }
                break;
            }

            case STMT_IF: {
                int lbl_else = next_label(cg);
                int lbl_end = next_label(cg);
                gen_expr(cg, s->as.if_stmt.cond);
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tJUMPZ L_else_%d, $2\n", lbl_else);

                symtab_push_scope(cg->st);
                gen_stmt_list(cg, &s->as.if_stmt.then_block);
                symtab_pop_scope(cg->st);

                fprintf(cg->out, "\tJUMP L_endif_%d\n", lbl_end);
                fprintf(cg->out, "LABEL L_else_%d\n", lbl_else);

                symtab_push_scope(cg->st);
                gen_stmt_list(cg, &s->as.if_stmt.else_block);
                symtab_pop_scope(cg->st);

                fprintf(cg->out, "LABEL L_endif_%d\n", lbl_end);
                break;
            }

            case STMT_WHILE: {
                int lbl_cond = next_label(cg);
                int lbl_end = next_label(cg);
                fprintf(cg->out, "LABEL L_while_cond_%d\n", lbl_cond);
                gen_expr(cg, s->as.while_stmt.cond);
                pop_reg(cg, "$2");
                fprintf(cg->out, "\tJUMPZ L_while_end_%d, $2\n", lbl_end);

                symtab_push_scope(cg->st);
                gen_stmt_list(cg, &s->as.while_stmt.body);
                symtab_pop_scope(cg->st);

                fprintf(cg->out, "\tJUMP L_while_cond_%d\n", lbl_cond);
                fprintf(cg->out, "LABEL L_while_end_%d\n", lbl_end);
                break;
            }

            case STMT_RETURN: {
                if (s->as.ret_stmt.value) {
                    gen_expr(cg, s->as.ret_stmt.value);
                    pop_reg(cg, "$1");
                }
                emit_epilogue(cg);
                break;
            }

            case STMT_EXPR: {
                Expr* expr = s->as.expr_stmt.expr;
                gen_expr(cg, expr);
                if (expr->inferred_type && strcmp(expr->inferred_type, "void") != 0) {
                    fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
                }
                break;
            }
        }
    }
}

static void emit_runtime_helpers(CodeGen* cg) {
    /* 1. Affichage d'un chunk chaîne ($0-2 = chunk) */
    emit_prologue(cg, "L_rt_print_str", 0);
    fprintf(cg->out, "\tSET $2, [$0-2]\n");
    fprintf(cg->out, "\tJUMPZ L_pstr_done, $2\n");
    fprintf(cg->out, "\tGETSIZE $3, $2\n");
    fprintf(cg->out, "\tSET $4, 0\n");
    fprintf(cg->out, "LABEL L_pstr_loop\n");
    fprintf(cg->out, "\tLTI $5, $4, $3\n");
    fprintf(cg->out, "\tJUMPZ L_pstr_done, $5\n");
    fprintf(cg->out, "\tGETWORD $6, $2, $4\n");
    fprintf(cg->out, "\tWRITES $6\n");
    fprintf(cg->out, "\tADDI $4, $4, 1\n");
    fprintf(cg->out, "\tJUMP L_pstr_loop\n");
    fprintf(cg->out, "LABEL L_pstr_done\n");
    emit_epilogue(cg);

    /* 2. Concaténation de 2 chunks chaîne ([$0-3] + [$0-2]) */
    emit_prologue(cg, "L_rt_concat", 0);
    fprintf(cg->out, "\tSET $2, [$0-3]\n");
    fprintf(cg->out, "\tSET $3, [$0-2]\n");
    fprintf(cg->out, "\tGETSIZE $4, $2\n");
    fprintf(cg->out, "\tGETSIZE $5, $3\n");
    fprintf(cg->out, "\tADDI $6, $4, $5\n");
    fprintf(cg->out, "\tCREATE $1, $6\n");
    fprintf(cg->out, "\tSET $6, 0\n");
    fprintf(cg->out, "LABEL L_cat_l1\n");
    fprintf(cg->out, "\tLTI $7, $6, $4\n");
    fprintf(cg->out, "\tJUMPZ L_cat_l2_init, $7\n");
    fprintf(cg->out, "\tGETWORD $7, $2, $6\n");
    fprintf(cg->out, "\tSETWORD $1, $6, $7\n");
    fprintf(cg->out, "\tADDI $6, $6, 1\n");
    fprintf(cg->out, "\tJUMP L_cat_l1\n");
    fprintf(cg->out, "LABEL L_cat_l2_init\n");
    fprintf(cg->out, "\tSET $6, 0\n");
    fprintf(cg->out, "LABEL L_cat_l2\n");
    fprintf(cg->out, "\tLTI $7, $6, $5\n");
    fprintf(cg->out, "\tJUMPZ L_cat_done, $7\n");
    fprintf(cg->out, "\tGETWORD $7, $3, $6\n");
    fprintf(cg->out, "\tADDI $2, $4, $6\n");
    fprintf(cg->out, "\tSETWORD $1, $2, $7\n");
    fprintf(cg->out, "\tADDI $6, $6, 1\n");
    fprintf(cg->out, "\tJUMP L_cat_l2\n");
    fprintf(cg->out, "LABEL L_cat_done\n");
    emit_epilogue(cg);

    /* 3. Conversion int -> chunk chaîne ([$0-2] = int) */
    emit_prologue(cg, "L_rt_int2str", 0);
    fprintf(cg->out, "\tSET $2, [$0-2]\n");
    fprintf(cg->out, "\tEQI $3, $2, 0\n");
    fprintf(cg->out, "\tJUMPZ L_i2s_nonzero, $3\n");
    fprintf(cg->out, "\tCREATE $1, 1\n");
    fprintf(cg->out, "\tSETWORD $1, 0, \"0\"\n");
    emit_epilogue(cg);

    fprintf(cg->out, "LABEL L_i2s_nonzero\n");
    fprintf(cg->out, "\tSET $3, 0\n");
    fprintf(cg->out, "\tLTI $4, $2, 0\n");
    fprintf(cg->out, "\tJUMPZ L_i2s_pos, $4\n");
    fprintf(cg->out, "\tSET $3, 1\n");
    fprintf(cg->out, "\tSUBI $2, 0, $2\n");
    fprintf(cg->out, "LABEL L_i2s_pos\n");
    fprintf(cg->out, "\tSET $4, 0\n");
    fprintf(cg->out, "LABEL L_i2s_div\n");
    fprintf(cg->out, "\tJUMPZ L_i2s_build, $2\n");
    fprintf(cg->out, "\tDIVI $5, $2, 10\n");
    fprintf(cg->out, "\tMULI $6, $5, 10\n");
    fprintf(cg->out, "\tSUBI $6, $2, $6\n");
    fprintf(cg->out, "\tINT2STRING $7, $6\n");
    push_reg(cg, "$7");
    fprintf(cg->out, "\tADDI $4, $4, 1\n");
    fprintf(cg->out, "\tSET $2, $5\n");
    fprintf(cg->out, "\tJUMP L_i2s_div\n");

    fprintf(cg->out, "LABEL L_i2s_build\n");
    fprintf(cg->out, "\tADDI $5, $4, $3\n");
    fprintf(cg->out, "\tCREATE $1, $5\n");
    fprintf(cg->out, "\tSET $6, 0\n");
    fprintf(cg->out, "\tJUMPZ L_i2s_pop, $3\n");
    fprintf(cg->out, "\tSETWORD $1, 0, \"-\"\n");
    fprintf(cg->out, "\tSET $6, 1\n");
    fprintf(cg->out, "LABEL L_i2s_pop\n");
    fprintf(cg->out, "\tJUMPZ L_i2s_done, $4\n");
    pop_reg(cg, "$7");
    fprintf(cg->out, "\tSETWORD $1, $6, $7\n");
    fprintf(cg->out, "\tADDI $6, $6, 1\n");
    fprintf(cg->out, "\tSUBI $4, $4, 1\n");
    fprintf(cg->out, "\tJUMP L_i2s_pop\n");
    fprintf(cg->out, "LABEL L_i2s_done\n");
    emit_epilogue(cg);

    /* 4. Comparaison lexicographique de 2 chunks chaîne ([$0-3] vs [$0-2]) -> -1, 0, 1 */
    emit_prologue(cg, "L_rt_strcmp", 0);
    fprintf(cg->out, "\tSET $2, [$0-3]\n");
    fprintf(cg->out, "\tSET $3, [$0-2]\n");
    fprintf(cg->out, "\tGETSIZE $4, $2\n");
    fprintf(cg->out, "\tGETSIZE $5, $3\n");
    fprintf(cg->out, "\tSET $6, 0\n");
    fprintf(cg->out, "LABEL L_cmp_loop\n");
    fprintf(cg->out, "\tLTI $7, $6, $4\n");
    fprintf(cg->out, "\tJUMPZ L_cmp_end1, $7\n");
    fprintf(cg->out, "\tLTI $7, $6, $5\n");
    fprintf(cg->out, "\tJUMPZ L_cmp_gt, $7\n");
    fprintf(cg->out, "\tGETWORD $7, $2, $6\n");
    fprintf(cg->out, "\tGETWORD $1, $3, $6\n");
    fprintf(cg->out, "\tLTS $7, $7, $1\n");
    fprintf(cg->out, "\tJUMPNZ L_cmp_lt, $7\n");
    fprintf(cg->out, "\tGETWORD $7, $2, $6\n");
    fprintf(cg->out, "\tGETWORD $1, $3, $6\n");
    fprintf(cg->out, "\tGTS $7, $7, $1\n");
    fprintf(cg->out, "\tJUMPNZ L_cmp_gt, $7\n");
    fprintf(cg->out, "\tADDI $6, $6, 1\n");
    fprintf(cg->out, "\tJUMP L_cmp_loop\n");
    fprintf(cg->out, "LABEL L_cmp_end1\n");
    fprintf(cg->out, "\tLTI $7, $6, $5\n");
    fprintf(cg->out, "\tJUMPNZ L_cmp_lt, $7\n");
    fprintf(cg->out, "\tSET $1, 0\n");
    emit_epilogue(cg);
    fprintf(cg->out, "LABEL L_cmp_lt\n");
    fprintf(cg->out, "\tSET $1, -1\n");
    emit_epilogue(cg);
    fprintf(cg->out, "LABEL L_cmp_gt\n");
    fprintf(cg->out, "\tSET $1, 1\n");
    emit_epilogue(cg);

    /* 5. readString() */
    emit_prologue(cg, "L_rt_read_str", 0);
    fprintf(cg->out, "\tREADS $2\n");
    fprintf(cg->out, "\tEQS $3, $2, \"\"\n");
    fprintf(cg->out, "\tJUMPZ L_rstr_nonempty, $3\n");
    emit_empty_string(cg, "$1");
    emit_epilogue(cg);
    fprintf(cg->out, "LABEL L_rstr_nonempty\n");
    fprintf(cg->out, "\tCREATE $1, 1\n");
    fprintf(cg->out, "\tSETWORD $1, 0, $2\n");
    emit_epilogue(cg);

    /* 6. Object.toString() */
    emit_prologue(cg, "L_m_Object_toString", 0);
    fprintf(cg->out, "\tSET $2, [$0-2]\n");
    push_reg(cg, "$2");
    fprintf(cg->out, "\tCALL [$SP+1], L_rt_int2str\n");
    fprintf(cg->out, "\tSUBI $SP, $SP, 1\n");
    emit_epilogue(cg);

    /* 7. Object.getClass() */
    emit_prologue(cg, "L_m_Object_getClass", 0);
    fprintf(cg->out, "\tSET $2, [$0-2]\n");
    fprintf(cg->out, "\tGETWORD $3, $2, 0\n");
    fprintf(cg->out, "\tGETWORD $1, $3, 1\n");
    emit_epilogue(cg);

    /* 8. subStr(s, i, n) */
    emit_prologue(cg, "L_builtin_substr", 0);
    fprintf(cg->out, "\tSET $2, [$0-2]\n");
    fprintf(cg->out, "\tSET $3, [$0-3]\n");
    fprintf(cg->out, "\tSET $4, [$0-4]\n");
    fprintf(cg->out, "\tGETSIZE $5, $2\n");
    fprintf(cg->out, "\tLTI $6, $3, 0\n");
    fprintf(cg->out, "\tJUMPNZ L_substr_empty, $6\n");
    fprintf(cg->out, "\tLTI $6, $3, $5\n");
    fprintf(cg->out, "\tJUMPZ L_substr_empty, $6\n");
    fprintf(cg->out, "\tGTI $6, $4, 0\n");
    fprintf(cg->out, "\tJUMPZ L_substr_empty, $6\n");
    fprintf(cg->out, "\tADDI $6, $3, $4\n");
    fprintf(cg->out, "\tGTI $7, $6, $5\n");
    fprintf(cg->out, "\tJUMPZ L_substr_alloc, $7\n");
    fprintf(cg->out, "\tSUBI $4, $5, $3\n");
    fprintf(cg->out, "LABEL L_substr_alloc\n");
    fprintf(cg->out, "\tCREATE $1, $4\n");
    fprintf(cg->out, "\tSET $6, 0\n");
    fprintf(cg->out, "LABEL L_substr_loop\n");
    fprintf(cg->out, "\tLTI $7, $6, $4\n");
    fprintf(cg->out, "\tJUMPZ L_substr_done, $7\n");
    fprintf(cg->out, "\tADDI $7, $3, $6\n");
    fprintf(cg->out, "\tGETWORD $7, $2, $7\n");
    fprintf(cg->out, "\tSETWORD $1, $6, $7\n");
    fprintf(cg->out, "\tADDI $6, $6, 1\n");
    fprintf(cg->out, "\tJUMP L_substr_loop\n");
    fprintf(cg->out, "LABEL L_substr_done\n");
    emit_epilogue(cg);
    fprintf(cg->out, "LABEL L_substr_empty\n");
    emit_empty_string(cg, "$1");
    emit_epilogue(cg);
}

void codegen_generate(Program* prog, SymTable* st, const char* output_path) {
    FILE* out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Internal error: cannot open output file %s\n", output_path);
        exit(19);
    }

    CodeGen cg = {out, st, NULL, NULL, 0};

    fprintf(out, "#! /bin/vypint\n");
    fprintf(out, "# VYPcode: 1.0\n");
    fprintf(out, "# Generated by: xarmenl00\n\n");
    fprintf(out, "\tSET $0, 0\n");
    fprintf(out, "\tSET $1, 0\n\n");

    /* 1. Chunk global des Method Tables stocké à la base de la pile [0] */
    fprintf(out, "\tCREATE $2, %zu\n", st->class_count);
    fprintf(out, "\tSET [0], $2\n");
    fprintf(out, "\tADDI $SP, $SP, 1\n\n");

    for (size_t i = 0; i < st->class_count; i++) {
        ClassInfo* cls = st->classes[i];
        fprintf(out, "\tCREATE $3, %zu\n", cls->method_count + 2);
        fprintf(out, "\tSETWORD $2, %zu, $3\n", i);

        if (cls->super_class) {
            int parent_idx = 0;
            for (size_t k = 0; k < st->class_count; k++) {
                if (st->classes[k] == cls->super_class) { parent_idx = (int)k; break; }
            }
            fprintf(out, "\tGETWORD $4, $2, %d\n", parent_idx);
            fprintf(out, "\tSETWORD $3, 0, $4\n");
        } else {
            fprintf(out, "\tSETWORD $3, 0, 0\n");
        }

        emit_string_literal(&cg, "$4", cls->name);
        fprintf(out, "\tSETWORD $3, 1, $4\n");

      for (size_t m = 0; m < cls->method_count; m++) {
         FuncInfo* minfo = cls->methods[m];
         fprintf(out, "\tSETWORD $3, %zu, \"L_m_%s_%s\"\n", m + 2, minfo->owner_class, minfo->name);
     }
    }

    /* 2. Point d'entrée : appel à main() puis arrêt propre */
    fprintf(out, "\n\tCALL [$SP+1], L_fn_main\n");
    fprintf(out, "\tJUMP L_program_exit\n");

    /* 3. Fonctions runtime intégrées */
    emit_runtime_helpers(&cg);

    /* 4. Fonctions globales utilisateur */
    for (size_t i = 0; i < prog->func_count; i++) {
        FuncDecl* fdecl = prog->functions[i];
        FuncInfo* finfo = symtab_find_func(st, fdecl->name);
        cg.cur_class = NULL;
        cg.cur_func = finfo;

        symtab_push_scope(st);
        for (size_t p = 0; p < fdecl->params.count; p++) {
            symtab_add_var(st, fdecl->params.items[p].name, fdecl->params.items[p].type, -1 - (int)p);
        }
        st->next_local_offset = 1;

        char fn_label[256];
        snprintf(fn_label, sizeof(fn_label), "L_fn_%s", fdecl->name);
        emit_prologue(&cg, fn_label, finfo->local_count);
        gen_stmt_list(&cg, &fdecl->body);

        if (strcmp(fdecl->return_type, "string") == 0) {
            emit_empty_string(&cg, "$1");
        } else if (strcmp(fdecl->return_type, "void") != 0) {
            fprintf(out, "\tSET $1, 0\n");
        }
        emit_epilogue(&cg);
        symtab_pop_scope(st);
    }

    /* 5. Méthodes des classes */
    for (size_t i = 1; i < st->class_count; i++) {
        ClassInfo* cls = st->classes[i];
        ClassDecl* cdecl = cls->ast_node;
        for (size_t m = 0; m < cdecl->method_count; m++) {
            FuncDecl* mdecl = cdecl->methods[m];
            FuncInfo* minfo = class_find_method(cls, mdecl->name);
            cg.cur_class = cls;
            cg.cur_func = minfo;

            symtab_push_scope(st);
            symtab_add_var(st, "this", cls->name, -1);
            for (size_t p = 0; p < mdecl->params.count; p++) {
                symtab_add_var(st, mdecl->params.items[p].name, mdecl->params.items[p].type, -2 - (int)p);
            }
            st->next_local_offset = 1;

            char m_label[256];
            snprintf(m_label, sizeof(m_label), "L_m_%s_%s", cls->name, mdecl->name);
            emit_prologue(&cg, m_label, minfo->local_count);
            gen_stmt_list(&cg, &mdecl->body);

            if (strcmp(mdecl->return_type, "string") == 0) {
                emit_empty_string(&cg, "$1");
            } else if (strcmp(mdecl->return_type, "void") != 0) {
                fprintf(out, "\tSET $1, 0\n");
            }
            emit_epilogue(&cg);
            symtab_pop_scope(st);
        }
    }

    fprintf(out, "\nLABEL L_program_exit\n");
    fclose(out);
}