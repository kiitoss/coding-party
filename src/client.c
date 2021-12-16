#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int main(int argc, char *argv[]) {
    int fm;
    key_t cle;
    int nb_chefs = atoi(argv[1]);

    printf("Je suis un client, il y a %d chefs\n", nb_chefs);
    
    connexion_fm("client", LETTRE_CODE_CLIENT, &cle, &fm);
    
    sleep(2);

    // envoie_requete(fm, ordre, duree);

    // requete_t rep = attend_reponse(fm, ordre);
    // printf("Retour du mecano %d\n", ordre);

    exit(EXIT_SUCCESS);
}