#include "../includes/semap-gestionnaire.h"

void init_semap(int cle, int *semap, unsigned short *val_init) {
    /* On cree le semaphore (meme cle) */
    *semap = semget(cle, 1, IPC_CREAT | 0660);
    if (*semap == -1){
        printf("Probleme creation ensemble de semaphore ou il existe deja\n");
        return;
    }

    /* On l'initialise */
    if (semctl(*semap, 0, SETALL, val_init) == -1){
        printf("Probleme initialisation semaphore\n");
        /* On detruit les IPC deje crees : */
        semctl(*semap, 0, IPC_RMID, NULL);
        *semap = -1;
        return;
    }
}

void deconnexion_semap(int semap) {
    semctl(semap, 0, IPC_RMID, NULL);
}