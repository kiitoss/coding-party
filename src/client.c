#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

void travail_termine() {
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int fm;
    key_t cle;
    // int nb_chefs = atoi(argv[1]);

    pid_t pid = getpid();

    mon_sigaction(SIGUSR1, arret);
    mon_sigaction(SIGUSR2, travail_termine);
    
    connexion_fm("client", LETTRE_CODE_CLIENT, &cle, &fm);
    
    printf("\t--> Client n°%d demande reparation\n", pid);
    fm_client_envoie_requete(fm, pid);

    pause();

    exit(EXIT_FAILURE);
}