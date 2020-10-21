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
                return contains_epsilon(r->first_son) || contains_epsilon(r->second_son);

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
                return plus(derivative(r->first_son, c), derivative(r->second_son, c));

            case CONCAT:
                if (contains_epsilon(r))
                    return plus(cat(derivative(r->first_son, c), r->second_son), derivative(r->second_son, c));
                else
                    return cat(derivative(r->first_son, c), r->second_son);

            case STAR:
                return cat(derivative(r->first_son, c), r);
        }
    }
}

/* renvoie 1 si une regex contient une chaine, et 0 sinon */
int match(regex r, char *s) {
    if (*s != '\0') {
        regex r2 = simplify_better(derivative(r, *s)); // Dérive r avec le premier caractère de s et simplifie
        match(r2, s + 1); // On test pour l'expression dérivé la chaine à partir du second caractère
    } else {
        if (r->regex_type == ZERO) return 0; // La chaine n'est pas reconnue
        if (r->regex_type == ONE) return 1; // La chaine est reconnue
        return 0;
    }
}


/* renvoie 1 si le langage d'une regex est vide, et 0 sinon */
int empty(regex r) {
    return -1;
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
 * la ligne de commandes */
void test() {
    regex r = cat(symbol('a'), symbol('a'));
    print_regex(r);
    char *s = "aaaa";
    printf(" reconnaît '%s' ? %d\n", s, match(r, s));

    regex r2 = star(cat(symbol('a'), symbol('b')));
    print_regex(r2);
    char *s2 = "abab";
    printf(" reconnaît '%s' ? %d\n", s2, match(r2, s2));
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
