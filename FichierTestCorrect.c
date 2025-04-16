 #include <stdio.h>
#include <stdlib.h>

 int main() {
    int a = 10;            // Déclaration et affectation
    float b = 3.14;        // Déclaration et affectation
    const int c = 5;       // Déclaration de constante

    a = 20;                // Affectation correcte
    b = 6.28;              // Affectation correcte

    // Déclaration d'un tableau
    int arr[5];            // Déclaration du tableau

    // Affectation d'une valeur à un tableau
    arr[0] = 100;          // Affectation correcte
    arr[1] = 200;          // Affectation correcte
    arr[2] = 300;          // Affectation correcte
    arr[3] = 400;          // Affectation correcte
    arr[4] = 500;          // Affectation correcte

    // Vérification des valeurs dans le tableau
    if (arr[2] > 250) {
        a = a + 5;         // Utilisation du tableau dans une condition
    }

    // Boucle for
    for (int i = 0; i < 10; i++) {
        b = b + 0.1;       // Boucle d'incrémentation
    }

    // Division correcte
    int x = 10;
    int y = 2;
    int z = x / y;         // Division correcte

    return 0;
    printf("Valeur de a : %d\n", a);
printf("Valeur de b : %.2f\n", b);
}
