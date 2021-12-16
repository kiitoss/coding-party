#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm;
    key_t cle;
    int ordre = atoi(argv[1]);
    
    connexion_fm_mecano("chef", &cle, &fm);
    
    srand(getpid());

    int duree = rand() % 5000 + 100;

    printf(".");

    sleep(2);

    printf("Envoie requete mecano %d / %d\n", ordre, duree);
    envoie_requete(fm, ordre, duree);

    requete_t rep = attend_reponse(fm, ordre);
    printf("Retour du mecano %d\n", ordre);

    exit(EXIT_SUCCESS);
}