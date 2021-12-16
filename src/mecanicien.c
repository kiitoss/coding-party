#include "../includes/global.h"
#include "../includes/smp-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int *tab = connexion_smp_mecano("mecanicien");

    printf(".");

    deconnexion_smp_mecano(tab);

    return EXIT_SUCCESS;
}