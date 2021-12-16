#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm;
    
    connexion_fm_mecano("mecanicien", NULL, &fm);

    printf(".");


    printf("Attend travail type %d\n", ordre);
    requete_t rep = attend_reponse(fm, ordre);
    
    
    usleep(rep.duree * 1000);
    if (ordre == 2) {
        sleep(40);
    }
    printf("Envoie requete fin travail\n");
    envoie_requete(fm, ordre, 0);
    

    exit(EXIT_SUCCESS);
}