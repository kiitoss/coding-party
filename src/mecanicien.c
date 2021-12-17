#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

void operation_outils(int semap, unsigned short *outils, int action) {
    struct sembuf *sops = (struct sembuf *) malloc(NB_OUTILS * sizeof(struct sembuf)); 

    for (int i = 0; i < NB_OUTILS; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = action;
        sops[i].sem_flg = SEM_UNDO;
    }

    semop(semap, sops, 1);

    free(sops);
}



int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm;
    key_t cle;

    requete_mecano_t requete;

    mon_sigaction(SIGUSR1, arret);
    
    connexion_fm("mecanicien", LETTRE_CODE_MECANO, &cle, &fm);


    /* Recuperation semaphore */
    int semap = semget(cle, 1, 0);
    if (semap == -1) {
        fprintf(stderr, "(mecanicien) Probleme recuperation semaphore\n");
        exit(EXIT_FAILURE);
    }
    

    printf("\t\t(%d) <-- Attend travail\n", ordre);
    fm_mecano_attend_requete(fm, &requete);

    operation_outils(semap, requete.outils, -1);

    printf("\t\t(%d) /!\\ Debut du travail\n", ordre);

    usleep(requete.duree * 1000);

    operation_outils(semap, requete.outils, 1);
    
    printf("\t\t--> (%d) Envoie requete fin travail\n", ordre);
    fm_mecano_envoie_reponse(fm, requete.type_reponse);

    exit(EXIT_SUCCESS);
}