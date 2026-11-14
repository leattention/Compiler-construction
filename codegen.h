
/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtrank00) 
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symtable.h"

void codegen_generate(Program* prog, SymTable* st, const char* output_path);

#endif