#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int line;
extern int col;

typedef enum {
    VARIABLE,
    CONSTANTE,
    TABLEAU
} CodeEntite;

typedef struct {
    char name[50];       // Nom de l'entité
    char type[10];       // Type (int, float, etc.)
    CodeEntite codeEntite;  // Code de l'entité (Variable, Constante, Tableau)
    int isConstant;      // 1 si c'est une constante, 0 sinon
    int size;            // -1 pour les scalaires, >0 pour les tableaux
    int valeur;          // Valeur de la constante (si c'est une constante)
} Symbol;

typedef enum {
    ERREUR_SYNTAXE,            // Erreur de syntaxe
    ERREUR_SA,                 // Erreur sémantique
    ERREUR_DIV_ZERO,           // Division par zéro
    ERREUR_IND_DECLARE,        // Identificateur non déclaré
    ERREUR_TYPE_INCOMPATIBLE,  // Incompatibilité de type
    ERREUR_DEPASSEMENT_BORNE   // Dépassement de bornes de tableau
} TypeErreur;

typedef struct {
    TypeErreur type;     // Type de l'erreur
    int ligne;           // Numéro de la ligne
    int colonne;         // Numéro de la colonne
    char message[100];   // Message d'erreur
    char entite[50];     // Entité concernée (par exemple, le nom de la variable)
} Erreur;

#define MAX_ERREURS 100
Erreur tableErreurs[MAX_ERREURS];
int countErreurs = 0;
#define MAX_SYMBOLS 100
Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void ajouterErreur(TypeErreur type, int ligne, int colonne, const char *message, const char *entite) {
    if (countErreurs < MAX_ERREURS) {
        tableErreurs[countErreurs].type = type;
        tableErreurs[countErreurs].ligne = ligne;
        tableErreurs[countErreurs].colonne = colonne;
        strncpy(tableErreurs[countErreurs].message, message, sizeof(tableErreurs[countErreurs].message) - 1);
        strncpy(tableErreurs[countErreurs].entite, entite, sizeof(tableErreurs[countErreurs].entite) - 1);
        countErreurs++;
    }
}

void afficherErreurs() {
    for (int i = 0; i < countErreurs; i++) {
        printf("Erreur %d : %s\n", i + 1, tableErreurs[i].message);
        printf("Entite : %s\n", tableErreurs[i].entite);
        printf("Type : ");
        switch (tableErreurs[i].type) {
            case ERREUR_SYNTAXE: printf("Erreur de syntaxe\n"); break;
            case ERREUR_SA: printf("Erreur semantique\n"); break;
            case ERREUR_DIV_ZERO: printf("Division par zero\n"); break;
            case ERREUR_IND_DECLARE: printf("Identificateur non declare\n"); break;
            case ERREUR_TYPE_INCOMPATIBLE: printf("Incompatibilite de type\n"); break;
            case ERREUR_DEPASSEMENT_BORNE: printf("Depassement de bornes\n"); break;
        }
        printf("Ligne : %d, Colonne : %d\n\n", tableErreurs[i].ligne, tableErreurs[i].colonne);
    }
}

void addSymbol(const char *name, const char *type, CodeEntite codeEntite, int isConstant, int size, int valeur) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            // Confusion entre un identificateur et un tableau
            if (symbolTable[i].codeEntite != codeEntite) {
                fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Identificateur '%s' de type incompatible.\n", line, col, name);
                return;
            }
            // Si l'identificateur est déjà déclaré, mais c'est une constante ou une variable, cela pose un problème
            if (symbolTable[i].isConstant && !isConstant) {
                fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Impossible de redéfinir '%s' comme variable après l'avoir défini comme constante.\n", line, col, name);
                return;
            }
            // Si c'est un tableau, on vérifie également la taille
            if (symbolTable[i].size != size) {
                fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Taille du tableau '%s' modifiée.\n", line, col, name);
                return;
            }
            return; // Le symbole existe déjà, donc on ne l'ajoute pas à nouveau
        }
    }

    // Ajout d'un nouveau symbole si pas de conflit
    strncpy(symbolTable[symbolCount].name, name, sizeof(symbolTable[symbolCount].name) - 1);
    strncpy(symbolTable[symbolCount].type, type, sizeof(symbolTable[symbolCount].type) - 1);
    symbolTable[symbolCount].codeEntite = codeEntite;
    symbolTable[symbolCount].isConstant = isConstant;
    symbolTable[symbolCount].size = size;
    symbolTable[symbolCount].valeur = valeur;

    symbolTable[symbolCount].name[sizeof(symbolTable[symbolCount].name) - 1] = '\0';
    symbolTable[symbolCount].type[sizeof(symbolTable[symbolCount].type) - 1] = '\0';

    symbolCount++;
}


Symbol* findSymbol(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}
void checkExpressionType(const char *expr1Type, const char *expr2Type, const char *operator) {
    if (strcmp(expr1Type, expr2Type) != 0) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Incompatibilite de type dans l'expression '%s'. Types incompatibles: %s et %s.\n",
                line, col, operator, expr1Type, expr2Type);
        ajouterErreur(ERREUR_TYPE_INCOMPATIBLE, line, col, "Incompatibilite de type dans l'expression.", "");
    }
}

void checkAssignment(const char *var, const char *exprType) {
    Symbol *s = findSymbol(var);
    if (!s) {
        // Ajout d'une erreur pour identificateur non déclaré
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Identificateur '%s' non declare.\n", line, col, var);
        ajouterErreur(ERREUR_IND_DECLARE, line, col, "Identificateur non declare.", var);
        return;
    }
    if (s->isConstant) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Impossible de modifier la constante '%s'.\n", line, col, var);
        ajouterErreur(ERREUR_SA, line, col, "Impossible de modifier la constante.", var);
    }
    if (strcmp(s->type, exprType) != 0) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Incompatibilite de type pour '%s'. Attendu: %s, Trouv\u00e9: %s.\n",
                line, col, var, s->type, exprType);
        ajouterErreur(ERREUR_TYPE_INCOMPATIBLE, line, col, "Incompatibilite de type", var);
    }
}


void checkArrayBounds(const char *arrayName, int index, int ligne, int colonne) {
    Symbol *s = findSymbol(arrayName);
    if (!s) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Tableau '%s' non declare.\n", ligne, colonne, arrayName);
        ajouterErreur(ERREUR_IND_DECLARE, ligne, colonne, "Tableau non declare.", arrayName);
        return;
    }
    if (s->size <= 0) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : '%s' n est pas un tableau.\n", ligne, colonne, arrayName);
        ajouterErreur(ERREUR_SA, ligne, colonne, "Ce n'est pas un tableau.", arrayName);
        return;
    }
    if (index >= s->size || index < 0) {
        fprintf(stderr, "Erreur semantique [ligne %d, colonne %d] : Depassement de bornes dans le tableau '%s'.\n", ligne, colonne, arrayName);
        ajouterErreur(ERREUR_DEPASSEMENT_BORNE, ligne, colonne, "Depassement de bornes", arrayName);
    }
}


void checkDivisionByZero(int divisor, int ligne, int colonne) {
    if (divisor == 0) {
        ajouterErreur(ERREUR_DIV_ZERO, ligne, colonne, "Division par zero detectee.", "diviseur");
    }
}

void checkTypeCompatibility(const char *var, const char *exprType, int ligne, int colonne) {
    Symbol *s = findSymbol(var);
    if (s && strcmp(s->type, exprType) != 0) {
        ajouterErreur(ERREUR_TYPE_INCOMPATIBLE, ligne, colonne, "Incompatibilite de type.", var);
    }
}

void printSymbolTable() {
    printf("\n---- Table des Symboles ----\n");
    for (int i = 0; i < symbolCount; i++) {
        char *entiteType;
        switch(symbolTable[i].codeEntite) {
            case VARIABLE: entiteType = "Variable"; break;
            case CONSTANTE: entiteType = "Constante"; break;
            case TABLEAU: entiteType = "Tableau"; break;
            default: entiteType = "Inconnu"; break;
        }

        printf("Nom: %s | Type: %s | Entite: %s | Constante: %s | Taille: %d | Valeur: %d\n",
            symbolTable[i].name,
            symbolTable[i].type,
            entiteType,
            symbolTable[i].isConstant ? "Oui" : "Non",
            symbolTable[i].size,
            symbolTable[i].valeur);
    }
    printf("-----------------------------\n");
}
