#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm_mecano, fm_client;
    key_t cle_mecano, cle_client;
    int ordre = atoi(argv[1]);
    requete_t rep;
    
    connexion_fm("chef", LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    // connexion_fm("chef", LETTRE_CODE_CLIENT, &cle_client, &fm_client);

    srand(getpid());
    int duree = rand() % 5000 + 100;

    printf(".");

    // mon_sigaction(SIGUSR1, fils_OK);

    // printf("Chef n°%d: attente client.\n", ordre);
    // rep = attend_reponse(fm_client, 1);
    

    printf("Envoie requete mecano %d / %d\n", ordre, duree);
    envoie_requete(fm_mecano, ordre, duree);

    sleep(10);

    rep = attend_reponse(fm_mecano, ordre);
    printf("Retour du mecano %d\n", ordre);

    exit(EXIT_SUCCESS);
}