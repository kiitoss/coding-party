#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm;
    key_t cle;

    int nsops = NB_OUTILS;
    struct sembuf *sops = (struct sembuf *) malloc(nsops * sizeof(struct sembuf)); 


    mon_sigaction(SIGUSR1, arret);

    for (int i = 0; i < nsops; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = -1;
        sops[i].sem_flg = SEM_UNDO;
    }
    
    connexion_fm("mecanicien", LETTRE_CODE_MECANO, &cle, &fm);

    /* Recuperation semaphore */
    int semap = semget(cle, 1, 0);
    if (semap == -1) {
        fprintf(stderr, "(mecanicien) Probleme recuperation semaphore\n");
        exit(EXIT_FAILURE);
    }
    
    printf(".");

    printf("\t\t(%d) <-- Attend travail\n", ordre);
    requete_mecano_t requete;
    fm_mecano_attend_requete(fm, &requete);

    printf("\t\t(%d) /!\\ Debut du travail: outils: ", ordre);
    for (int i=0; i < NB_OUTILS; i++) printf("%d ", requete.outils[i]);
    printf("\n");
    
    // if ((semop(semap, sops, nsops)) == -1) {
    //     fprintf(stderr, "(mecanicien) Operation sur semaphore echoue\n");
    //     exit(EXIT_FAILURE);
    // }
    
    usleep(requete.duree * 1000);
    
    
    printf("\t\t--> (%d) Envoie requete fin travail\n", ordre);
    fm_mecano_envoie_reponse(fm, requete.type_reponse);

    // for (int i = 0; i < nsops; i++) {
    //     sops[i].sem_op = +1;
    // }

    // if ((semop(semap, sops, nsops)) == -1) {
    //     fprintf(stderr, "(mecanicien) Operation sur semaphore echoue\n");
    //     exit(EXIT_FAILURE);
    // }
    

    exit(EXIT_SUCCESS);
}