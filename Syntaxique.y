%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *msg);
int yylex(void);
int nb_ligne = 1;
%}

%locations

%union {
    int ival;
    float fval;
    char *sval;
}

%token <sval> IDF
%token <ival> CHIFFRE
%token <fval> FLOAT

%token MAIN_PRGM L3_SOFTWARE VAR BEGIN_PG END_PG LET CONST
%token IF THEN ELSE FOR FROM TO WHILE STEP INPUT OUTPUT
%token PLUS MOINS MULT DIV AFFECT MODULO
%token INF SUP INFEG SUPEG EGAL EGALE DIFF
%token AND OR NOT
%token PAR_OUV PAR_FERM PVIRG VIRG
%token ACCO_OUV ACC_FERM
%token DEUX_POINTS CROCH_OUV CROCH_FERM
%token DEFINE
%token INT_TYPE FLOAT_TYPE
%token DO
%token EXPONENT
%token ACC_OUV
%token STRING


%left OR
%left AND
%right NOT
%left SUP SUPEG INF INFEG EGALE DIFF
%left PLUS MOINS
%left MULT DIV

%type <sval> type
%type <sval> expression
%type <sval> conditions

%%

programme:
    MAIN_PRGM L3_SOFTWARE PVIRG VAR declarations BEGIN_PG ACCO_OUV instructions ACC_FERM END_PG PVIRG
    {
        printf("Programme syntaxiquement correct.\n");
    }
    ;

declarations:
    | declarations declaration
    ;

declaration:
    LET liste_idf DEUX_POINTS type PVIRG
    {
        printf("Declaration a la ligne %d : variables de type %s.\n", @1.first_line, $4);
    }
    | DEFINE CONST IDF DEUX_POINTS type AFFECT expression PVIRG
    {
        printf("Constante '%s' de type %s definie a la ligne %d.\n", $3, $5, @3.first_line);
    }
    | VAR IDF CROCH_OUV CHIFFRE CROCH_FERM DEUX_POINTS type PVIRG
    {
        printf("Tableau '%s' de taille %d de type %s (ligne %d).\n", $2, $4, $7, @2.first_line);
    }
    | LET liste_idf DEUX_POINTS CROCH_OUV type CROCH_FERM CHIFFRE PVIRG
{
    printf("Tableaux de type %s, taille %d (ligne %d).\n", $5, $7, @1.first_line);
}
;

liste_idf:
    IDF
    | liste_idf ',' IDF
    ;

type:
    INT_TYPE     { $$ = "int"; }
    | FLOAT_TYPE { $$ = "float"; }
    ;

instructions:
    | instructions instruction
    ;

instruction:
    IDF AFFECT expression PVIRG
    {
        printf("Affectation de '%s' a la ligne %d, colonne %d.\n", $1, @1.first_line, @1.first_column);
    }
    | INPUT PAR_OUV IDF PAR_FERM PVIRG
    {
        printf("Instruction INPUT de '%s' (ligne %d).\n", $3, @1.first_line);
    }
    | OUTPUT PAR_OUV liste_expressions PAR_FERM PVIRG
    {
        printf("Instruction OUTPUT (ligne %d).\n", @1.first_line);
    }
    | condition
    | boucle
    ;

liste_expressions:
    expression
    | liste_expressions ',' expression
    ;

condition:
    IF PAR_OUV conditions PAR_FERM THEN ACCO_OUV instructions ACC_FERM ELSE ACCO_OUV instructions ACC_FERM
    {
        printf("Condition IF-THEN-ELSE a la ligne %d.\n", @1.first_line);
    }
    ;

boucle:
    DO ACCO_OUV instructions ACC_FERM WHILE PAR_OUV conditions PAR_FERM PVIRG
    {
        printf("Boucle DO-WHILE a la ligne %d.\n", @1.first_line);
    }
    | FOR IDF FROM CHIFFRE TO CHIFFRE STEP CHIFFRE ACCO_OUV instructions ACC_FERM
    {
        printf("Boucle FOR a la ligne %d.\n", @1.first_line);
    }
    ;

expression:
    expression PLUS expression    { $$ = "int"; }
    | expression MOINS expression { $$ = "int"; }
    | expression MULT expression  { $$ = "int"; }
    | expression DIV expression   { $$ = "int"; }
    | expression MODULO expression { $$ = "int"; }
    | expression AND expression   { $$ = "int"; }
    | expression OR expression    { $$ = "int"; }
    | NOT expression              { $$ = "int"; }
    | CHIFFRE                     { $$ = "int"; }
    | FLOAT                       { $$ = "float"; }
    | IDF                         { $$ = "inconnu"; }
    ;

conditions:
    expression SUPEG expression { $$ = "bool"; }
    | expression SUP expression   { $$ = "bool"; }
    | expression INF expression   { $$ = "bool"; }
    | expression INFEG expression { $$ = "bool"; }
    | expression EGALE expression { $$ = "bool"; }
    | expression DIFF expression  { $$ = "bool"; }
    | expression AND expression   { $$ = "bool"; }
    | expression OR expression    { $$ = "bool"; }
    | NOT expression              { $$ = "bool"; }
    | CHIFFRE                     { $$ = "bool"; }
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Erreur syntaxique a la ligne %d, colonne %d : %s\n", yylloc.first_line, yylloc.first_column, msg);
}

int main() {
    return yyparse();
}
