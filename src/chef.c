#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm_mecano, fm_client;
    key_t cle_mecano, cle_client;
    int ordre = atoi(argv[1]);
    reponse_mecano_t rep_mecano;
    requete_client_t req_client;

    int type_reponse_mecano = ordre + 1;
    
    mon_sigaction(SIGUSR1, arret);
    
    connexion_fm("chef", LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    connexion_fm("chef", LETTRE_CODE_CLIENT, &cle_client, &fm_client);

    srand(getpid());
    int duree = rand() % 5000 + 5000;
    int outils[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++) outils[i] = 1;

    printf(".");

    if (ordre == 1 || ordre == 3) {
        sleep(3);
    }
    printf("<-- Chef n°%d: attente client.\n", ordre);
    req_client = fm_client_attend_requete(fm_client);
    
    printf("--> Envoie requete aux mecanos\n");
    fm_mecano_envoie_requete(fm_mecano, type_reponse_mecano, duree, outils);

    rep_mecano = fm_mecano_attend_reponse(fm_mecano, type_reponse_mecano);
    printf("<-- Retour du mecano %d\n", ordre);

    printf("<-- Retour au client %d\n", req_client.pid_client);
    fm_client_envoie_reponse(fm_client, req_client.pid_client);

    exit(EXIT_SUCCESS);
}