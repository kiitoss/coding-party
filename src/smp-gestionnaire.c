#include "../includes/global.h"
#include "../includes/smp-gestionnaire.h"


int *connexion_smp_mecano(char *type_travailleur) {
    struct stat st;
    key_t cle_mecano;

    int mem_part_mecano;
    int *tab;

    /* On teste si le fichier cle existe dans le repertoire courant */
    if (stat(FICHIER_CLE, &st) == -1) {
	    fprintf(stderr,"(%s) Probleme creation fichier cle\n", type_travailleur);
        exit(EXIT_FAILURE);
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "(%s) Probleme creation cle\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    /* Recuperation SMP */
    mem_part_mecano = shmget(cle_mecano, sizeof(entree_smp_mecano), 0);
    if (mem_part_mecano == -1) {
	    fprintf(stderr, "(%s) Probleme recuperation SMP\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    /* Attachement de la memoire partagee */
    tab = shmat(mem_part_mecano, NULL, 0);
    if (tab == (int *) -1){
	    fprintf(stderr, "(%s) Probleme attachement SMP\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    return tab;
}


void deconnexion_smp_mecano(int *tab) {
    /* On nettoie */
    /* Detachement SMP */
    shmdt(tab);
}