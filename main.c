/**
 * Kevin Traini
 * L3-info-3
 * TP3 info502
 */
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}





// _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
/**
 * QUESTIONS
 *
 * Question 1 :
 * 1) La commande $ echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaa | grep -E "^(a?){30}a{30}$" ne renvoie rien
 * 2) Par contre, si on met 30 'a' dans echo (au lieu de 29), la commande nous renvoie les 30 'a'
 *
 * Question 2 :
 * 1) La chaine de 29 'a' n'est pas reconnu par le fichier python
 * 2) La chaine de 30 'a' est reconnu par le fichier python, mais cela demande un temps de calcul très long.
 *    Ce doit être du à la structure du regex : celui ci spécifie que l'on peut avoir entre 0 et 30 'a' suivis de 30 'a'.
 *    Or le moteur regex va tout d'abord vérifier la condition 1 'a' suivi de 30 'a', puis 2 'a' suivi de 30 'a' ainsi de suite
 *    avant de revenir à 0 'a' suivi de 30 'a' qui est le bon schéma. On pourrait donc optimiser le regex comme ceci : "^a{30}(a?){30}"
 *
 * Question 3 :
 * 1) L'expression régulière "^(a?){30}a{30}$" reconnaît les mots composés d'une chaine de 0 à 30 'a' suivis de 30 'a'
 * 2) En Kleene cela ferais : "(1 + a + aa + aaa + aaaa + aaaaa + ...) . aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa". La partie entre parenthèse est exactement le mot vide ou 1 a ou 2 a ou 3 a, etc.
 *
 */


