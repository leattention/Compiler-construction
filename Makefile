# 1. Paramètres à personnaliser
$Login1  = "xarmenl00"
$Author1 = "Armengol ($Login1)"
$Author2 = "Tran (xtranke00)" 

# 2. Réécriture propre du Makefile (en LF Unix)
$MakefileContent = @"
# Project: VYPa Compiler (vypcomp)
# Authors: $Author1, $Author2

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -D_POSIX_C_SOURCE=200809L
FLEX = flex
BISON = bison

OBJS = main.o parser.tab.o lex.yy.o ast.o symtable.o semantic.o codegen.o

all: vypcomp

vypcomp: `$(OBJS)
	`$(CC) `$(CFLAGS) -o `$@ `$(OBJS)

parser.tab.c parser.tab.h: parser.y
	`$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: scanner.l parser.tab.h
	`$(FLEX) -o lex.yy.c scanner.l

%.o: %.c
	`$(CC) `$(CFLAGS) -c `$< -o `$@

clean:
	rm -f `$(OBJS) parser.tab.c parser.tab.h lex.yy.c vypcomp vypcomp.exe out*.vc

.PHONY: all clean
"@ -replace "`r`n", "`n"

[System.IO.File]::WriteAllText("$PWD\Makefile", $MakefileContent)

# 3. Remplacement automatique de "xlogin00" et des noms d'auteurs dans les sources
$SourceFiles = @(
    "main.c", "scanner.l", "parser.y",
    "ast.c", "ast.h",
    "symtable.c", "symtable.h",
    "semantic.c", "semantic.h",
    "codegen.c", "codegen.h"
)

foreach ($f in $SourceFiles) {
    if (Test-Path $f) {
        $text = [System.IO.File]::ReadAllText("$PWD\$f")
        $text = $text -replace "Votre Nom \(xlogin00\), Binome Nom \(xlogin01\)", "$Author1, $Author2"
        $text = $text -replace "xlogin00", $Login1
        $text = $text -replace "`r`n", "`n"
        [System.IO.File]::WriteAllText("$PWD\$f", $text)
    }
}

# 4. Création des archives propres (.tgz et .zip) sans fichiers temporaires
$StageDir = ".\_release_stage"
if (Test-Path $StageDir) { Remove-Item -Recurse -Force $StageDir }
New-Item -ItemType Directory -Path $StageDir | Out-Null

Copy-Item "Makefile" "$StageDir\"
foreach ($f in $SourceFiles) {
    if (Test-Path $f) { Copy-Item $f "$StageDir\" }
}

if (Test-Path "$Login1.tgz") { Remove-Item -Force "$Login1.tgz" }
if (Test-Path "$Login1.zip") { Remove-Item -Force "$Login1.zip" }

tar -czf "$Login1.tgz" -C $StageDir .
Compress-Archive -Path "$StageDir\*" -DestinationPath "$Login1.zip"
Remove-Item -Recurse -Force $StageDir

Write-Host "Tout est mis a jour et archive dans $Login1.tgz et $Login1.zip !" -ForegroundColor Green