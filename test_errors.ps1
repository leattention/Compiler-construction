$Compiler = ".\vypcomp.exe"
$TmpDir = ".\_tmp_err_tests"

if (-not (Test-Path $Compiler)) {
    Write-Host "Erreur: $Compiler introuvable. Lancez 'make' d'abord." -ForegroundColor Red
    exit 1
}

New-Item -ItemType Directory -Force -Path $TmpDir | Out-Null

$Passed = 0
$Failed = 0

function Run-ErrorTest {
    param (
        [string]$Name,
        [int]$ExpectedCode,
        [string]$CodeContent,
        [string]$CustomInputPath = "",
        [string]$CustomOutputPath = ""
    )

    $InFile = if ($CustomInputPath -ne "") { $CustomInputPath } else { Join-Path $TmpDir "test.vyp" }
    $OutFile = if ($CustomOutputPath -ne "") { $CustomOutputPath } else { Join-Path $TmpDir "out.vc" }

    if ($CustomInputPath -eq "") {
        Set-Content -Path $InFile -Value $CodeContent -Encoding UTF8
    }
    if (Test-Path $OutFile) { Remove-Item $OutFile -Force }

    & $Compiler $InFile $OutFile 2>$null
    $ActualCode = $LASTEXITCODE

    if ($ActualCode -eq $ExpectedCode) {
        Write-Host "[PASS] $Name -> Code $ActualCode" -ForegroundColor Green
        $script:Passed++
    } else {
        Write-Host "[FAIL] $Name -> Attendu: $ExpectedCode | Obtenu: $ActualCode" -ForegroundColor Red
        $script:Failed++
    }
}

Write-Host "=== Lancement de la batterie de tests des codes d'erreur VYPa ===" -ForegroundColor Cyan

# --- Code 0 : Programme valide ---
Run-ErrorTest -Name "00 - Programme valide minimal" -ExpectedCode 0 -CodeContent @"
void main(void) {
    int x;
    x = 42;
    print(x);
}
"@

# --- Code 11 : Erreurs lexicales ---
Run-ErrorTest -Name "11 - Caractere illegal (@)" -ExpectedCode 11 -CodeContent @"
void main(void) {
    int x;
    x = 10 @ 2;
}
"@

Run-ErrorTest -Name "11 - Chaine litterale non fermee" -ExpectedCode 11 -CodeContent @"
void main(void) {
    string s;
    s = "bonjour;
}
"@

# --- Code 12 : Erreurs syntaxiques ---
Run-ErrorTest -Name "12 - Point-virgule manquant" -ExpectedCode 12 -CodeContent @"
void main(void) {
    int x
    x = 5;
}
"@

Run-ErrorTest -Name "12 - Parenthese manquante dans if" -ExpectedCode 12 -CodeContent @"
void main(void) {
    if (1 == 1 {
        print(1);
    } else {}
}
"@

# --- Code 13 : Erreurs semantiques de types ---
Run-ErrorTest -Name "13 - Affectation string dans int" -ExpectedCode 13 -CodeContent @"
void main(void) {
    int x;
    x = "texte";
}
"@

Run-ErrorTest -Name "13 - Addition int + string" -ExpectedCode 13 -CodeContent @"
void main(void) {
    int x;
    x = 5 + "abc";
}
"@

Run-ErrorTest -Name "13 - Mauvais type d'argument de fonction" -ExpectedCode 13 -CodeContent @"
int f(int a) { return a; }
void main(void) {
    int r;
    r = f("pas_un_int");
}
"@

# --- Code 14 : Autres erreurs semantiques ---
Run-ErrorTest -Name "14 - Variable non declaree" -ExpectedCode 14 -CodeContent @"
void main(void) {
    y = 10;
}
"@

Run-ErrorTest -Name "14 - Redeclaration de variable dans la meme portee" -ExpectedCode 14 -CodeContent @"
void main(void) {
    int x;
    int x;
}
"@

Run-ErrorTest -Name "14 - Absence de fonction main" -ExpectedCode 14 -CodeContent @"
int helper(void) {
    return 1;
}
"@

Run-ErrorTest -Name "14 - Classe parente inexistante" -ExpectedCode 14 -CodeContent @"
class Enfant : ParentInexistant {
    int a;
}
void main(void) {}
"@

Run-ErrorTest -Name "14 - Methode inexistante sur un objet" -ExpectedCode 14 -CodeContent @"
class A : Object {
    int a;
}
void main(void) {
    A obj;
    obj = new A;
    obj.methodeInconnue();
}
"@

# --- Code 19 : Erreurs internes / fichiers ---
Run-ErrorTest -Name "19 - Fichier source inexistant" -ExpectedCode 19 -CodeContent "" `
    -CustomInputPath ".\fichier_qui_nexiste_pas_12345.vyp"

# Nettoyage du dossier temporaire
Remove-Item -Recurse -Force $TmpDir

Write-Host "`n=== Resultat : $Passed reussis, $Failed echoues ===" -ForegroundColor Cyan