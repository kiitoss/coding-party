#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm = connexion_fm_mecano("chef");
    srand(getpid());
    int duree = rand() % 5000 + 100;

    printf(".");

    sleep(5);

    printf("Envoie requete mecano %d / %d\n", ordre, duree);
    envoie_requete(fm, ordre, duree);
    requete_t rep = attend_reponse(fm, ordre);
    printf("Retour du mecano %d\n", ordre);
    exit(EXIT_SUCCESS);
}