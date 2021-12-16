#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm = connexion_fm_mecano("chef");

    printf(".");

    sleep(5);
    printf("Envoie requete mecano\n");
    envoie_requete(fm, ordre);
    reponse_t rep = attend_reponse(fm);
    printf("Retour du mecano\n");
    exit(EXIT_SUCCESS);
}