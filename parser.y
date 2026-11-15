/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
extern int yylineno;
Program* root_program = NULL;

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error (line %d): %s\n", yylineno, s);
    exit(12);
}
%}

%union {
    int64_t intVal;
    char* strVal;
    Program* prog;
    FuncDecl* func;
    ClassDecl* cls;
    ParamList paramList;
    StmtList stmtList;
    Stmt* stmt;
    StringList idList;
    Expr* expr;
    ExprList exprList;
}

%token T_CLASS T_ELSE T_IF T_INT T_NEW T_RETURN T_STRING T_SUPER T_THIS T_VOID T_WHILE
%token T_EQ T_NEQ T_LEQ T_GEQ T_AND T_OR
%token <intVal> T_INT_LIT
%token <strVal> T_STRING_LIT T_ID

%type <prog> program
%type <func> func_def
%type <cls> class_def class_members
%type <strVal> type data_type prim_type
%type <paramList> param_list param_items
%type <stmtList> stmt_list block
%type <stmt> stmt var_decl
%type <idList> id_list
%type <expr> expr logic_or_expr logic_and_expr eq_expr rel_expr add_expr mul_expr unary_expr postfix_expr primary_expr
%type <exprList> arg_list arg_items

%start program

%%

program
    : /* vide */ { root_program = ast_program_new(); $$ = root_program; }
    | program func_def { ast_program_add_func($1, $2); $$ = $1; }
    | program class_def { ast_program_add_class($1, $2); $$ = $1; }
    ;

func_def
    : type T_ID '(' param_list ')' block
      { $$ = ast_func_new(yylineno, $1, $2, $4, $6); }
    ;

type
    : data_type { $$ = $1; }
    | T_VOID    { $$ = strdup("void"); }
    ;

data_type
    : prim_type { $$ = $1; }
    | T_ID      { $$ = $1; }
    ;

prim_type
    : T_INT     { $$ = strdup("int"); }
    | T_STRING  { $$ = strdup("string"); }
    ;

param_list
    : T_VOID      { $$ = (ParamList){NULL, 0, 0}; }
    | param_items { $$ = $1; }
    ;

param_items
    : data_type T_ID
      {
          $$ = (ParamList){NULL, 0, 0};
          param_list_push(&$$, $1, $2);
      }
    | param_items ',' data_type T_ID
      {
          param_list_push(&$1, $3, $4);
          $$ = $1;
      }
    ;

class_def
    : T_CLASS T_ID ':' T_ID '{' class_members '}'
      {
          $6->name = $2;
          $6->super_name = $4;
          $$ = $6;
      }
    ;

class_members
    : /* vide */ { $$ = ast_class_new(yylineno); }
    | class_members data_type id_list ';'
      {
          ast_class_add_field($1, ast_stmt_var_decl(yylineno, $2, $3));
          $$ = $1;
      }
    | class_members data_type T_ID '(' param_list ')' block
      {
          ast_class_add_method($1, ast_func_new(yylineno, $2, $3, $5, $7));
          $$ = $1;
      }
    | class_members T_VOID T_ID '(' param_list ')' block
      {
          ast_class_add_method($1, ast_func_new(yylineno, strdup("void"), $3, $5, $7));
          $$ = $1;
      }
    ;

block
    : '{' stmt_list '}' { $$ = $2; }
    ;

stmt_list
    : /* vide */ { $$ = (StmtList){NULL, 0, 0}; }
    | stmt_list stmt { stmt_list_push(&$1, $2); $$ = $1; }
    ;

var_decl
    : data_type id_list ';'
      { $$ = ast_stmt_var_decl(yylineno, $1, $2); }
    ;

id_list
    : T_ID
      {
          $$ = (StringList){NULL, 0, 0};
          string_list_push(&$$, $1);
      }
    | id_list ',' T_ID
      {
          string_list_push(&$1, $3);
          $$ = $1;
      }
    ;

stmt
    : var_decl { $$ = $1; }
    | postfix_expr '=' expr ';'
      { $$ = ast_stmt_assign(yylineno, $1, $3); }
    | T_IF '(' expr ')' block T_ELSE block
      { $$ = ast_stmt_if(yylineno, $3, $5, $7); }
    | T_WHILE '(' expr ')' block
      { $$ = ast_stmt_while(yylineno, $3, $5); }
    | T_RETURN ';'
      { $$ = ast_stmt_return(yylineno, NULL); }
    | T_RETURN expr ';'
      { $$ = ast_stmt_return(yylineno, $2); }
    | postfix_expr '(' arg_list ')' ';'
      { $$ = ast_stmt_expr(yylineno, ast_expr_call(yylineno, $1, $3)); }
    ;

expr
    : logic_or_expr { $$ = $1; }
    ;

logic_or_expr
    : logic_and_expr { $$ = $1; }
    | logic_or_expr T_OR logic_and_expr
      { $$ = ast_expr_binary(yylineno, strdup("||"), $1, $3); }
    ;

logic_and_expr
    : eq_expr { $$ = $1; }
    | logic_and_expr T_AND eq_expr
      { $$ = ast_expr_binary(yylineno, strdup("&&"), $1, $3); }
    ;

eq_expr
    : rel_expr { $$ = $1; }
    | eq_expr T_EQ rel_expr
      { $$ = ast_expr_binary(yylineno, strdup("=="), $1, $3); }
    | eq_expr T_NEQ rel_expr
      { $$ = ast_expr_binary(yylineno, strdup("!="), $1, $3); }
    ;

rel_expr
    : add_expr { $$ = $1; }
    | rel_expr '<' add_expr
      { $$ = ast_expr_binary(yylineno, strdup("<"), $1, $3); }
    | rel_expr T_LEQ add_expr
      { $$ = ast_expr_binary(yylineno, strdup("<="), $1, $3); }
    | rel_expr '>' add_expr
      { $$ = ast_expr_binary(yylineno, strdup(">"), $1, $3); }
    | rel_expr T_GEQ add_expr
      { $$ = ast_expr_binary(yylineno, strdup(">="), $1, $3); }
    ;

add_expr
    : mul_expr { $$ = $1; }
    | add_expr '+' mul_expr
      { $$ = ast_expr_binary(yylineno, strdup("+"), $1, $3); }
    | add_expr '-' mul_expr
      { $$ = ast_expr_binary(yylineno, strdup("-"), $1, $3); }
    ;

mul_expr
    : unary_expr { $$ = $1; }
    | mul_expr '*' unary_expr
      { $$ = ast_expr_binary(yylineno, strdup("*"), $1, $3); }
    | mul_expr '/' unary_expr
      { $$ = ast_expr_binary(yylineno, strdup("/"), $1, $3); }
    ;

unary_expr
    : postfix_expr { $$ = $1; }
    | '!' unary_expr
      { $$ = ast_expr_unary(yylineno, strdup("!"), $2); }
    | '(' prim_type ')' unary_expr
      { $$ = ast_expr_cast(yylineno, $2, $4); }
    | '(' T_ID ')' unary_expr
      { $$ = ast_expr_cast(yylineno, $2, $4); }
    ;

postfix_expr
    : primary_expr { $$ = $1; }
    | postfix_expr '.' T_ID
      { $$ = ast_expr_member(yylineno, $1, $3); }
    | postfix_expr '(' arg_list ')'
      { $$ = ast_expr_call(yylineno, $1, $3); }
    ;

primary_expr
    : T_INT_LIT    { $$ = ast_expr_int(yylineno, $1); }
    | T_STRING_LIT { $$ = ast_expr_str(yylineno, $1); }
    | T_ID         { $$ = ast_expr_id(yylineno, $1); }
    | T_THIS       { $$ = ast_expr_id(yylineno, strdup("this")); }
    | T_SUPER      { $$ = ast_expr_id(yylineno, strdup("super")); }
    | T_NEW T_ID   { $$ = ast_expr_new(yylineno, $2); }
    | '(' expr ')' { $$ = $2; }
    ;

arg_list
    : /* vide */ { $$ = (ExprList){NULL, 0, 0}; }
    | arg_items  { $$ = $1; }
    ;

arg_items
    : expr
      {
          $$ = (ExprList){NULL, 0, 0};
          expr_list_push(&$$, $1);
      }
    | arg_items ',' expr
      {
          expr_list_push(&$1, $3);
          $$ = $1;
      }
    ;

%%