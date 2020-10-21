/**
 * Kevin Traini
 * L3-info-3
 * TP3 info502
 */
#include "regex.h"
#include <stdio.h>

/*
 * un cas qui ne marche pas bien dans la plupart des bibliothèques de regex
 * modernes : matcher a...a sur (a?)...(a?)a...a
 * La fonction renvoie la regex "(a?){n}a{n}" en syntaxe Kleene
 */
regex corner(int n) {
    regex r = one();
    for (int i = 0; i < n; i++) {
        r = cat(plus(one(), symbol('a')), r);
    }
    for (int i = 0; i < n; i++) {
        r = cat(r, symbol('a'));
    }
    return r;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        test();
        return 0;
    }

    int n = atoi(argv[1]);
    size_t size = 1024;
    char *line = malloc(size * sizeof(char));

    regex r = corner(n);
    print_regex(r);
    printf("\n");
    int l;

    while (getline(&line, &size, stdin) != -1) {
        l = strlen(line);
        if (line[l - 1] == '\n') line[l - 1] = 0;
        if (match(r, line)) {
            printf("contient '%s': oui\n", line);
        } else {
            printf("contient '%s': non\n", line);
        }
    }
    return 0;
}


