// test erreur
#include <stdio.h>
#include <stdlib.h>

int main() {
    int a = 10;            // Déclaration et affectation
    float b = 3.14;        // Déclaration et affectation
    const int c = 5;       // Déclaration de constante

    a = 20;                // Affectation correcte
    b = 6.28;              // Affectation correcte

    // Erreur : tentative de modification d'une constante
    c = 10;                // Erreur sémantique (constante modifiée)

    // Déclaration d'un tableau
    int arr[5];            // Déclaration du tableau

    // Affectation d'une valeur à un tableau
    arr[0] = 100;          // Affectation correcte
    arr[1] = 200;          // Affectation correcte
    arr[2] = 300;          // Affectation correcte
    arr[3] = 400;          // Affectation correcte
    arr[5] = 500;          // Erreur sémantique (dépassement de borne)

    // Erreur : tentative d'affectation d'un type incompatible
    a = 3.5;               // Erreur sémantique (incompatibilité de type)

    // Division par zéro
    int x = 0;
    int y = 10;
    int z = y / x;         // Erreur sémantique (division par zéro)


   //Erreur:  idf déja déclaré
    int x=2

    // Utilisation d'un tableau dans une condition
    if (arr[2] > 250) {
        b = b + 1.0;       // Utilisation correcte
    }

    // Erreur : accès à un tableau non déclaré
    arr[10] = 1000;        // Erreur sémantique (dépassement de borne)

    return 0;
}
