#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int main(int argc, char *argv[]) {
    srand(getpid());  

    requete_client_t req_client;
    reponse_mecano_t rep_mecano;
    key_t cle_mecano, cle_client;

    int fm_mecano, fm_client;

    int ordre = atoi(argv[1]);
    long type_reponse_mecano = ordre + REQUETE_TYPE_TAF;
    int duree = rand() % 5000 + 5000;

    int *outils = malloc(sizeof(int) * NB_OUTILS);

    for (int i = 0; i < NB_OUTILS; i++) outils[i] = atoi(argv[1 + i]);
    

    mon_sigaction(SIGUSR1, arret);
    

    connexion_fm("chef", LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    connexion_fm("chef", LETTRE_CODE_CLIENT, &cle_client, &fm_client);


    printf("<-- Chef n°%d: attente client.\n", ordre);
    fm_client_attend_requete(fm_client, &req_client);
    
    printf("--> Envoie requete aux mecanos\n");
    fm_mecano_envoie_requete(fm_mecano, type_reponse_mecano, duree, outils);
    
    fm_mecano_attend_reponse(fm_mecano, type_reponse_mecano, &rep_mecano);
    printf("<-- Retour du mecano %d\n", ordre);

    printf("<-- Retour au client %d\n", req_client.pid_client);
    kill(req_client.pid_client, SIGUSR2);


    exit(EXIT_SUCCESS);
}