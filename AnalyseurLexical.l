%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Syntaxique.tab.h"

int line = 1;
int column = 1;

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = line; \
    yylloc.first_column = column; \
    yylloc.last_column = column + yyleng - 1; \
    column += yyleng;

%}

Float           [-+]?[0-9]+\.[0-9]+
idf             [a-zA-Z](_?[a-zA-Z0-9]){0,13}
chiffre         [-+]?[0-9]+

%x COMMENT_MULTILIGNE

%%

"MainPrgm"      { return MAIN_PRGM; }
"L3_software"   { return L3_SOFTWARE; }
"Var"           { return VAR; }
"BeginPg"       { return BEGIN_PG; }
"EndPg"         { return END_PG; }
"let"           { return LET; }
"Int"           { return INT_TYPE; }
"Float"         { return FLOAT_TYPE; }
"if"            { return IF; }
"then"          { return THEN; }
"else"          { return ELSE; }
"do"            { return DO; }
"while"         { return WHILE; }
"for"           { return FOR; }
"from"          { return FROM; }
"to"            { return TO; }
"step"          { return STEP; }
"input"         { return INPUT; }
"output"        { return OUTPUT; }
"@define"       { return DEFINE; }
"Const"         { return CONST; }

"=="            { return EGALE; }
"!="            { return DIFF; }
">="            { return SUPEG; }
"<="            { return INFEG; }
">"             { return SUP; }
"<"             { return INF; }
"="             { return EGAL; }

"+"             { return PLUS; }
"-"             { return MOINS; }
"*"             { return MULT; }
"/"             { return DIV; }
"%"             { return MODULO; }
":"             { return DEUX_POINTS; }
":="            { return AFFECT; }
"AND"           { return AND; }
"OR"            { return OR; }
"!"             { return NOT; }

"("             { return PAR_OUV; }
")"             { return PAR_FERM; }
"["             { return CROCH_OUV; }
"]"             { return CROCH_FERM; }
";"             { return PVIRG; }
","             { return VIRG; }
"{"             { return ACCO_OUV; }
"}"             { return ACC_FERM; }

{Float} {
    yylval.fval = atof(yytext);
    return FLOAT;
}

{chiffre} {
    yylval.ival = atoi(yytext);
    return CHIFFRE;
}

{idf} {
    yylval.sval = strdup(yytext);
    return IDF;
}

"<!-"([^-\n]|-([^>]|$))*"-!>"   { /* commentaire ligne ignoré */ }

"{--"           { BEGIN(COMMENT_MULTILIGNE); }

<COMMENT_MULTILIGNE>"--}" { BEGIN(INITIAL); }
<COMMENT_MULTILIGNE>\n    { line++; column = 1; }
<COMMENT_MULTILIGNE>.     { /* Ignorer */ }

[ \t]+          { column += yyleng; /* espaces ignorés */ }
\n              { line++; column = 1; }

. {
    printf("Erreur lexicale à la ligne %d, colonne %d : caractère inconnu '%s'\n", line, column, yytext);
    column += yyleng;
}

%%

int yywrap(void)
{ return 1; }
