#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int main(int argc, char *argv[]) {
    if (argc != 2 + NB_OUTILS) exit(EXIT_FAILURE);

    srand(getpid());

    requete_client_t req_client;
    reponse_mecano_t rep_mecano;
    key_t cle_mecano, cle_client;

    int fm_mecano, fm_client;

    int ordre = atoi(argv[1]);
    long type_reponse_mecano = ordre + REQUETE_TYPE_TAF;
    int duree = rand() % 5000 + 5000;

    unsigned short *outils = malloc(sizeof(unsigned short) * NB_OUTILS);

    for (int i = 0; i < NB_OUTILS; i++) outils[i] = atoi(argv[2 + i]);
    

    desactive_signaux();
    mon_sigaction(SIGUSR1, arret);
    

    connexion_fm("chef", LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    if (fm_mecano == -1) exit(EXIT_FAILURE);

    connexion_fm("chef", LETTRE_CODE_CLIENT, &cle_client, &fm_client);
    if (fm_client == -1) exit(EXIT_FAILURE);


    for (;;) {
        couleur(ROUGE);
        fprintf(stdout, "Chef n°%d: Disponible\n", ordre);
        
        fm_client_attend_requete(fm_client, &req_client);

        couleur(ROUGE);
        fprintf(stdout, "Chef n°%d: Reçoit une demande du client n°%d\n", ordre, req_client.pid_client);
        
        couleur(ROUGE);
        fprintf(stdout, "Chef n°%d: Envoie requete aux mecanos\n", ordre);
        fm_mecano_envoie_requete(fm_mecano, type_reponse_mecano, duree, outils);
        
        
        fm_mecano_attend_reponse(fm_mecano, type_reponse_mecano, &rep_mecano);
        couleur(ROUGE);
        fprintf(stdout, "Chef n°%d: Recoit la reponse du mecano\n", ordre);

        couleur(ROUGE);
        fprintf(stdout, "Chef n°%d: Retour au client n°%d\n", ordre, req_client.pid_client);
        kill(req_client.pid_client, SIGUSR2);
    }

    free(outils);

    exit(EXIT_SUCCESS);
}