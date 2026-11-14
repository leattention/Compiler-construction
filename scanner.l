/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ast.h"
#include "parser.tab.h"

static void lex_error(const char* msg) {
    fprintf(stderr, "Lexical error (line %d): %s\n", yylineno, msg);
    exit(11);
}

static char* str_buf = NULL;
static size_t str_len = 0;
static size_t str_cap = 0;

static void buf_reset(void) {
    str_len = 0;
    if (!str_buf) {
        str_cap = 64;
        str_buf = malloc(str_cap);
        if (!str_buf) exit(19);
    }
    str_buf[0] = '\0';
}

static void buf_append_str(const char* s) {
    size_t n = strlen(s);
    while (str_len + n + 1 > str_cap) {
        str_cap *= 2;
        str_buf = realloc(str_buf, str_cap);
        if (!str_buf) exit(19);
    }
    memcpy(str_buf + str_len, s, n + 1);
    str_len += n;
}

static void buf_append_char(char c) {
    char tmp[2] = {c, '\0'};
    buf_append_str(tmp);
}
%}

%option noyywrap
%option yylineno
%option nounput noinput

%x COMMENT
%x STRING

DIGIT       [0-9]
NONZERO     [1-9]
HEX         [0-9a-fA-F]
ID          [a-zA-Z_][a-zA-Z0-9_]*

%%

[ \t\r\n]+              { /* Ignorer les espaces */ }
"//".*                  { /* Commentaire ligne */ }

"/*"                    { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n             { }
<COMMENT>.              { }
<COMMENT><<EOF>>        { lex_error("Unterminated block comment"); }

\"                      { buf_reset(); BEGIN(STRING); }
<STRING>\"              {
                            BEGIN(INITIAL);
                            yylval.strVal = strdup(str_buf);
                            if (!yylval.strVal) exit(19);
                            return T_STRING_LIT;
                        }
<STRING>\\n             { buf_append_str("\\n"); }
<STRING>\\t             { buf_append_str("\\t"); }
<STRING>\\\\            { buf_append_str("\\\\"); }
<STRING>\\\"            { buf_append_str("\\\""); }
<STRING>\\x{HEX}{6}     { buf_append_str(yytext); }
<STRING>\\.             { lex_error("Invalid escape sequence"); }
<STRING>\n              { lex_error("Unescaped newline in string literal"); }
<STRING><<EOF>>         { lex_error("Unterminated string literal"); }
<STRING>.               {
                            unsigned char c = (unsigned char)yytext[0];
                            if (c <= 31) lex_error("Control character in string literal");
                            buf_append_char(yytext[0]);
                        }

"class"                 { return T_CLASS; }
"else"                  { return T_ELSE; }
"if"                    { return T_IF; }
"int"                   { return T_INT; }
"new"                   { return T_NEW; }
"return"                { return T_RETURN; }
"string"                { return T_STRING; }
"super"                 { return T_SUPER; }
"this"                  { return T_THIS; }
"void"                  { return T_VOID; }
"while"                 { return T_WHILE; }

"=="                    { return T_EQ; }
"!="                    { return T_NEQ; }
"<="                    { return T_LEQ; }
">="                    { return T_GEQ; }
"&&"                    { return T_AND; }
"||"                    { return T_OR; }

"<"                     { return '<'; }
">"                     { return '>'; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"!"                     { return '!'; }
"="                     { return '='; }
"."                     { return '.'; }
","                     { return ','; }
";"                     { return ';'; }
":"                     { return ':'; }
"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }

0|{NONZERO}{DIGIT}*     {
                            errno = 0;
                            char* endptr = NULL;
                            long long val = strtoll(yytext, &endptr, 10);
                            if (errno == ERANGE || *endptr != '\0') {
                                lex_error("Integer literal out of 64-bit range");
                            }
                            yylval.intVal = (int64_t)val;
                            return T_INT_LIT;
                        }
0{DIGIT}+               { lex_error("Leading zeros in integer literal"); }

{ID}                    {
                            yylval.strVal = strdup(yytext);
                            if (!yylval.strVal) exit(19);
                            return T_ID;
                        }

.                       { lex_error("Unexpected character"); }

%%