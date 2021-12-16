#include <stdio.h>
#include <stdlib.h>

#define NB_OUTILS 4
#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

/* Fonction d'usage du programme */
void usage(char *s) {
    fprintf(stderr, "Usage: %s <nb_chefs> <nb_mecanos>", s);
    for (int i = 1; i <= NB_OUTILS; i++) {
        fprintf(stderr, " nb_%d", i);
    }
    fprintf(stderr, "\n\tnb_chefs: nombre de chefs ( > %d )", MIN_CHEFS - 1);
    fprintf(stderr, "\n\tnb_mecanos: nombre de mecanos ( > %d )", MIN_MECANOS - 1);
    fprintf(stderr, "\n\tnb_i: nombre d'outils ( > %d )\n", MIN_OUTILS - 1);
    exit(EXIT_FAILURE);
}

/* Fonction principale du programme */
int main(int argc, char *argv[]) {
    int nb_chefs, nb_mecanos;
    int outils[NB_OUTILS];
    int param_valides = 1;

    /* Verficiation des parametres */
    if (argc < 3 + NB_OUTILS) {
        usage(argv[0]);
    }

    nb_chefs = atoi(argv[1]);
    if (nb_chefs < MIN_CHEFS) usage(argv[0]);

    nb_mecanos = atoi(argv[2]);
    if (nb_mecanos < MIN_MECANOS) usage(argv[0]);

    for (int i = 0; i < NB_OUTILS; i++) {
        outils[i] = atoi(argv[3+i]);
        if (outils[i] < MIN_OUTILS) {
            param_valides = 0;
            break;
        }
    }
    if (!param_valides) usage(argv[0]);
}