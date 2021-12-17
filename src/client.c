#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm;
    key_t cle;
    int nb_chefs = atoi(argv[1]);
    requete_client_t rep;
    pid_t pid = getpid();

    mon_sigaction(SIGUSR1, arret);
    
    connexion_fm("client", LETTRE_CODE_CLIENT, &cle, &fm);
    
    printf("\t--> Client n°%d demande reparation\n", pid);
    fm_client_envoie_requete(fm, REQUETE_TYPE_TRAVAIL, pid);

    rep = fm_client_attend_reponse(fm, pid);
    printf("\t<-- Retour du chef au client %d\n", pid);

    exit(EXIT_SUCCESS);
}