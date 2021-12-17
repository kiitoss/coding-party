#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

void travail_termine() {
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Quitte l'etablissement\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int fm;
    key_t cle;
    int nb_chefs = atoi(argv[1]);

    pid_t pid = getpid();

    if (argc != 2) exit(EXIT_FAILURE);

    desactive_signaux();
    mon_sigaction(SIGUSR1, arret);
    mon_sigaction(SIGUSR2, travail_termine);
    
    connexion_fm("client", LETTRE_CODE_CLIENT, &cle, &fm);
    if (fm == -1) exit(EXIT_FAILURE);
    
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Apporte une nouvelle voiture\n", pid);
    fm_client_envoie_requete(fm, pid);

    pause();

    exit(EXIT_FAILURE);
}