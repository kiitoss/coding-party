#include "global.h"


int main(int argc, char *argv[]) {
    struct stat st;
    key_t cle_mecano;

    int mem_part_mecano;
    int *tab;

    int ordre = atoi(argv[1]);

    /* On teste si le fichier cle existe dans le repertoire courant */
    if (stat(FICHIER_CLE, &st) == -1) {
	    fprintf(stderr,"(chef) Probleme creation fichier cle\n");
        exit(EXIT_FAILURE);
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "(chef) Probleme creation cle\n");
	    exit(EXIT_FAILURE);
    }

    /* Recuperation SMP */
    mem_part_mecano = shmget(cle_mecano, sizeof(entree_smp_mecano), 0);
    if (mem_part_mecano == -1) {
	    fprintf(stderr, "(chef) Probleme recuperation SMP\n");
	    exit(EXIT_FAILURE);
    }

    /* Attachement de la memoire partagee */
    tab = shmat(mem_part_mecano, NULL, 0);
    if (tab == (int *) -1){
	    fprintf(stderr, "Probleme attachement SMP\n");
	    exit(EXIT_FAILURE);
    }

    printf(".");

    /* On nettoie */
    /* Detachement SMP */
    shmdt(tab);

    return EXIT_SUCCESS;
}