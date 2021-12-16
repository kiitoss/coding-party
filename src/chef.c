#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    key_t cle;
    int fm;
    int ordre = atoi(argv[1]);
    
    connexion_fm_mecano("chef", &cle, &fm);
    
    srand(getpid());
    int duree = rand() % 5000 + 100;

    /* Recuperation semaphore */
    // int semap = semget(cle, 1, 0);
    // if (semap == -1) {
    //     fprintf(stderr, "(chef) Probleme recuperation semaphore\n");
    //     exit(EXIT_FAILURE);
    // }

    // semop(semap, &P, 1);

    printf(".");

    sleep(2);

    printf("Envoie requete mecano %d / %d\n", ordre, duree);
    envoie_requete(fm, ordre, duree);
    requete_t rep = attend_reponse(fm, ordre);
    printf("Retour du mecano %d\n", ordre);
    exit(EXIT_SUCCESS);
}