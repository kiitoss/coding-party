#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm;
    key_t cle;

    int nsops = NB_OUTILS;
    struct sembuf *sops = (struct sembuf *) malloc(nsops * sizeof(struct sembuf)); 

    for (int i = 0; i < nsops; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = -1;
        sops[i].sem_flg = SEM_UNDO;
    }
    
    connexion_fm_mecano("mecanicien", &cle, &fm);

    /* Recuperation semaphore */
    int semap = semget(cle, 1, 0);
    if (semap == -1) {
        fprintf(stderr, "(mecanicien) Probleme recuperation semaphore\n");
        exit(EXIT_FAILURE);
    }
    
    printf(".");

    printf("Attend travail type %d\n", ordre);
    requete_t rep = attend_reponse(fm, ordre);
    
    
    // if ((semop(semap, sops, nsops)) == -1) {
    //     fprintf(stderr, "(mecanicien) Operation sur semaphore echoue\n");
    //     exit(EXIT_FAILURE);
    // }
    
    usleep(rep.duree * 1000);
    
    
    printf("Envoie requete fin travail\n");
    envoie_requete(fm, ordre, 0);

    for (int i = 0; i < nsops; i++) {
        sops[i].sem_op = +1;
    }

    // if ((semop(semap, sops, nsops)) == -1) {
    //     fprintf(stderr, "(mecanicien) Operation sur semaphore echoue\n");
    //     exit(EXIT_FAILURE);
    // }
    

    exit(EXIT_SUCCESS);
}