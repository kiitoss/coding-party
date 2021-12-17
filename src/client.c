#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int *tab;

void travail_termine() {
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Quitte l'etablissement\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int nb_chefs = atoi(argv[1]);
    int fm = atoi(argv[2]);
    

    pid_t pid = getpid();

    if (argc != 3) exit(EXIT_FAILURE);

    desactive_signaux();
    mon_sigaction(SIGUSR1, arret);
    mon_sigaction(SIGUSR2, travail_termine);
    
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Apporte une nouvelle voiture aux %d chefs d'ateliers\n", pid, nb_chefs);

    fm_client_envoie_requete(fm, pid);

    pause();

    exit(EXIT_FAILURE);
}