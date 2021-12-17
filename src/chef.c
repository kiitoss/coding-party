#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm_mecano, fm_client;
    key_t cle_mecano, cle_client;
    int ordre = atoi(argv[1]);
    requete_mecano_t rep_mecano;
    requete_client_t rep_client;
    
    connexion_fm("chef", LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    connexion_fm("chef", LETTRE_CODE_CLIENT, &cle_client, &fm_client);

    srand(getpid());
    int duree = rand() % 5000 + 5000;

    printf(".");

    printf("<-- Chef n°%d: attente client.\n", ordre);
    rep_client = fm_client_attend_reponse(fm_client, REQUETE_TYPE_TRAVAIL);
    
    printf("--> Envoie requete mecano %d / %d\n", ordre, duree);
    fm_mecano_envoie_requete(fm_mecano, ordre, duree);

    rep_mecano = fm_mecano_attend_reponse(fm_mecano, ordre);
    printf("<-- Retour du mecano %d\n", ordre);

    printf("<-- Retour au client %d\n", rep_client.pid_client);
    fm_client_envoie_requete(fm_client, rep_client.pid_client, rep_client.pid_client);

    exit(EXIT_SUCCESS);
}