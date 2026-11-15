/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtranke00) 
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "semantic.h"
#include "codegen.h"

extern FILE* yyin;
extern int yyparse(void);
extern Program* root_program;

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Internal error: invalid arguments. Usage: vypcomp input [output]\n");
        return 19;
    }

    const char* input_path = argv[1];
    const char* output_path = (argc == 3) ? argv[2] : "out.vc";

    yyin = fopen(input_path, "r");
    if (!yyin) {
        fprintf(stderr, "Internal error: cannot open input file %s\n", input_path);
        return 19;
    }

    yyparse();
    fclose(yyin);

    SymTable* st = semantic_analyze(root_program);
    codegen_generate(root_program, st, output_path);

    return 0;
}