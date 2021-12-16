#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int ordre = atoi(argv[1]);
    int fm = connexion_fm_mecano("mecanicien");

    printf(".");

    

    printf("Attend travail type %d\n", ordre);
    reponse_t rep = attend_reponse(fm, ordre);
    
    
    sleep(5);
    if (ordre == 2) {
        sleep(40);
    }
    printf("Envoie requete fin travail\n");
    envoie_requete(fm, ordre);
    

    exit(EXIT_SUCCESS);
}