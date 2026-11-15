/* Project: VYPa Compiler (vypcomp)
 * Authors: Armengol Léa(xarmenl00), Tran Kenny (xtranke00) 
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symtable.h"

SymTable* semantic_analyze(Program* prog);

#endif