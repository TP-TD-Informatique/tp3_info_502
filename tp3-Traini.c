/**********************************************************************
 *** fichier à rendre pour le TP3 d'info502 (théorie des langages)
 *** étudiant: Kevin Traini
 **********************************************************************/


#include "regex.h"

/* affichage naif d'une regex */
void print_regex(regex r) {
    if (r == NULL) {
        fprintf(stderr, "ERREUR dans print_regex\n");
        exit(1);
    };
    switch (r->regex_type) {
        case ZERO:
            printf("0");
            break;
        case ONE:
            printf("1");
            break;
        case SYMBOL:
            putchar(r->symbol);
            break;
        case PLUS:
            print_regex(r->first_son);
            printf(" + ");
            print_regex(r->second_son);
            break;
        case CONCAT:
            if (r->first_son->regex_type == PLUS) {
                printf("(");
                print_regex(r->first_son);
                printf(").");
            } else {
                print_regex(r->first_son);
            }
            if (r->second_son->regex_type == PLUS) {
                printf("(");
                print_regex(r->second_son);
                printf(")");
            } else {
                print_regex(r->second_son);
            }
            break;
        case STAR:
            if (r->first_son->regex_type == PLUS || r->first_son->regex_type == CONCAT) {
                printf("(");
                print_regex(r->first_son);
                printf(")*");
            } else {
                print_regex(r->first_son);
                printf("*");
            }
            break;
        default:
            fprintf(stderr, "ERREUR dans print_regex\n");
            exit(1);
    }
}

/* renvoie 1 si une regex contient le mot vide, et 0 sinon */
int contains_epsilon(regex r) {
    if (r == NULL) {
        return 0;
    } else {
        switch (r->regex_type) {
            case ONE:
                return 1;

            case ZERO:
                return 0;

            case SYMBOL:
                return 0;

            case PLUS:
                return contains_epsilon(r->first_son) || contains_epsilon(r->second_son);

            case CONCAT:
                return contains_epsilon(r->first_son) && contains_epsilon(r->second_son);

            case STAR:
                return 1;
        }
    }
}

/* calcul la dérivé d'une regex selon un symbol */
regex derivative(regex r, char c) {
    if (r == NULL) {
        return r;
    } else {
        switch (r->regex_type) {
            case ZERO:
                return zero();

            case ONE:
                return zero();

            case SYMBOL:
                if (r->symbol == c)
                    return one();
                else
                    return zero();

            case PLUS:
                return smart_plus(derivative(r->first_son, c), derivative(r->second_son, c));

            case CONCAT:
                if (contains_epsilon(r))
                    return smart_plus(smart_cat(derivative(r->first_son, c), r->second_son),
                                      derivative(r->second_son, c));
                else
                    return smart_cat(derivative(r->first_son, c), r->second_son);

            case STAR:
                return smart_cat(derivative(r->first_son, c), r);
        }
    }
}

/* renvoie 1 si une regex contient une chaine, et 0 sinon */
int match(regex r, char *s) {
    //print_regex(r);printf("\n\n");
    // Traitement avant la dérivation pour cas remarquables
    if (r == NULL) return 0; // Si on n'a pas de regex
    if (r->regex_type == ZERO) return 0; // 0 ne reconnait jamais s
    if (r->regex_type == ONE) {
        if (*s == '\0') return 1; // 1 reconnait la chaine vide
        return 0; // 1 ne reconnait pas une chaine non vide
    }

    // A partir de la on a un regex utilisable et une chaine

    if (*s != '\0') { // Si la chaine est non vide
        r = simplify_better(derivative(r, *s)); // Dérive r avec le premier caractère de s
        match(r, s + 1); // On test pour l'expression dérivé la chaine à partir du second caractère
    } else { // Si la chaine est vide
        if (contains_epsilon(r)) return 1;
        return 0; // Si le regex ne contient pas 1, il ne reconnait pas la chaine vide
    }
}

regex smart_plus(regex r1, regex r2) {
    if (r1->regex_type == ZERO)
        return r2;
    if (r2->regex_type == ZERO)
        return r1;

    return plus(r1, r2);
}

regex smart_cat(regex r1, regex r2) {
    if (r1->regex_type == ZERO || r2->regex_type == ZERO)
        return zero();
    if (r1->regex_type == ONE)
        return r2;
    if (r2->regex_type == ONE)
        return r1;

    return cat(r1, r2);
}

regex smart_star(regex r1) {
    if (r1->regex_type == ZERO || r1->regex_type == ONE)
        return one();
    if (r1->regex_type == STAR)
        return r1;

    return star(r1);
}


/* renvoie 1 si le langage d'une regex est vide, et 0 sinon */
int empty(regex r) {
    return r == NULL;
}


/* renvoie 1 si le langage d'une regex est infini, et 0 sinon */
int infinite(regex r) {
    return -1;
}


/* calcule une regex qui reconnait le renversé d'une autre regex */
regex reverse(regex r) {
    return NULL;
}


/* calcule une regex qui reconnait les préfixes d'une autre regex */
regex prefix(regex r) {
    return NULL;
}

/* calcule une regex qui reconnait les suffixes d'une autre regex */
regex suffix(regex r) {
    return NULL;
}


/* la fonction de test qui est appelée lorsqu'aucun argument n'est donné sur
 * la ligne de commandes
*/
void test() {
    regex r = cat(plus(one(), symbol('a')), cat(one(), symbol('a')));
    print_regex(r);
    char *s = "aaa";
    printf(" reconnaît '%s' ? %d\n", s, match(r, s));
}

// Question 5
void test_2() {
    regex r = cat(star(symbol('a')), star(plus(plus(cat(cat(symbol('a'), symbol('a')), symbol('b')),
                                                    cat(symbol('b'), cat(star(symbol('b')), symbol('a')))),
                                               cat(symbol('b'), symbol('b')))));
    print_regex(r);
}

// Question 6
void test_3() {
    regex r1 = one();
    regex r2 = cat(star(symbol('a')), star(plus(plus(cat(cat(symbol('a'), symbol('a')), one()),
                                                     cat(symbol('b'), cat(star(symbol('b')), symbol('a')))),
                                                cat(zero(), symbol('b')))));
    regex r3 = cat(zero(), plus(symbol('a'), symbol('b')));
    printf("Contient 1 ? %d", contains_epsilon(r3));
}

// Question 7
void test_4() {
    regex r = cat(symbol('a'), symbol('a'));
    print_regex(derivative(r, 'a'));
    printf("\n");
    regex r2 = plus(star(symbol('a')), star(symbol('b')));
    print_regex(derivative(r2, 'a'));
}

// Question 8
void test_5() {
    regex r = cat(symbol('a'), symbol('a'));
    print_regex(r);
    char *s = "aaaa";
    printf(" reconnaît '%s' ? %d\n", s, match(r, s));

    regex r2 = star(cat(symbol('a'), symbol('b')));
    print_regex(r2);
    char *s2 = "abab";
    printf(" reconnaît '%s' ? %d\n", s2, match(r2, s2));
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
 * 2) En Kleene cela ferais : "(1 + a + aa + aaa + aaaa + aaaaa + ...) . aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa".
 *    La partie entre parenthèse est exactement le mot vide ou 1 a ou 2 a ou 3 a, etc.
 *
 * Question 9 :
 * 1) Les 29 'a' ne sont pas reconnus.
 * 2) Les 30 'a' ne sont pas reconnus.
 * En affichant les dérivées de la fontion match, on remarque qu'elles contiennent beaucoup de +0,
 * donc notre fonction match ne peut pas reconnaître la chaine. Il faudrait simplifier la dérivée.
 *
 * Question 10 :
 * Même en simplifiant les dérivée via les fonctions smart_, notre regex (celui pour les 30 'a') ne reconnait pas la chaine de 30 'a'.
 */