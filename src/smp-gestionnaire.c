#include "../includes/smp-gestionnaire.h"

void init_smp(key_t cle, int *tab, int *mem_part) {
    /* On cree le SMP et on teste s'il existe deja */
    *mem_part = shmget(cle, sizeof(int), IPC_CREAT | 0660);
    if (*mem_part == -1){
        printf("Probleme creation SMP ou il existe deja\n");
        *mem_part = -1;
        return;
    }

    /* Attachement de la memoire partagee */
    tab = shmat(*mem_part, NULL, 0);
    if (tab == (int *) -1){
        printf("Probleme attachement SMP\n");
        /* Il faut detruire le SMP puisqu'on l'a cree */
        shmctl(*mem_part, IPC_RMID,NULL);
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