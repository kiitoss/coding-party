#include "../includes/smp-gestionnaire.h"

static void attache_memoire_partagee(int *tab, int *mem_part) {
    /* Attachement de la memoire partagee */
    tab = shmat(*mem_part, NULL, 0);
    if (tab == (int *) -1) {
        fprintf(stderr, "Probleme attachement SMP\n");
        /* Il faut detruire le SMP puisqu'on l'a cree */
        shmctl(*mem_part, IPC_RMID,NULL);
        *mem_part = -1;
        return;
    }
}

void init_smp(key_t cle, int *tab, size_t taille, int *mem_part) {
    printf("here: %ld\n", taille);
    /* On cree le SMP et on teste s'il existe deja */
    *mem_part = shmget(cle, taille, IPC_CREAT | 0660);
    if (*mem_part == -1) {
        fprintf(stderr, "Probleme creation SMP ou il existe deja\n");
        *mem_part = -1;
        return;
    }

    attache_memoire_partagee(tab, mem_part);
}

void connexion_smp(key_t cle, int *mem_part) {
    /* Recuperation de la SMP */
    *mem_part = shmget(cle, sizeof(int), 0);
    
    if (*mem_part == -1) {
        fprintf(stderr, "Probleme connexion SMP\n");
        *mem_part = -1;
        return;
    }
}

void deconnexion_smp(int *tab, int mem_part) {
    /* Detachement SMP */
    shmdt(tab);

    /* Destruction SMP */
    shmctl(mem_part, IPC_RMID,NULL);
}