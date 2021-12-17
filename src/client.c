#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"
#include "../includes/smp-gestionnaire.h"

int *tab;

void travail_termine() {
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Quitte l'etablissement\n", getpid());
    exit(EXIT_SUCCESS);
}

int trouve_ordre_chef_moins_occupe(int nb_chefs) {
    printf("debut fonc %d\n", nb_chefs);
    int min = tab[1];
    int index_min = 1;
    printf("debut for");
    for (int i = 1; i <= nb_chefs; i++) {
        printf("%d\n", tab[i]);
        if (tab[i] < min) {
            min = tab[i];
            index_min = i;
        }
    }

    printf("%d\n", nb_chefs);
    tab[index_min]++;
    return index_min;
}

int main(int argc, char *argv[]) {
    int nb_chefs = atoi(argv[1]);
    int fm = atoi(argv[2]);
    int smp = atoi(argv[3]);
    int chef_moins_occupe;
    

    pid_t pid = getpid();

    if (argc != 4) exit(EXIT_FAILURE);

    int *tab = shmat(smp, NULL, 0);
    if (tab == (int *) -1) {
        fprintf(stderr, "Probleme attachement SMP du client\n");
        exit(EXIT_FAILURE);
    }

    desactive_signaux();
    mon_sigaction(SIGUSR1, arret);
    mon_sigaction(SIGUSR2, travail_termine);
    
    couleur(VERT);
    fprintf(stdout, "Client n°%d: Apporte une nouvelle voiture\n", pid);

    chef_moins_occupe = trouve_ordre_chef_moins_occupe(nb_chefs);

    printf("chef moins occupé: %d\n", chef_moins_occupe);
    fm_client_envoie_requete(fm, chef_moins_occupe, pid);

    tab[chef_moins_occupe + 1]++;

    pause();

    exit(EXIT_FAILURE);
}