#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"


int connexion_fm_mecano(char *type_travailleur) {
    key_t cle_mecano;
    int fm;
    FILE *fich_cle;


    /* Creation de la cle :          */
    /* 1 - On teste si le fichier cle existe dans le repertoire courant : */
    fich_cle = fopen(FICHIER_CLE,"r");
    if (fich_cle == NULL) {
        fprintf(stderr, "(%s) Lancement client impossible\n", type_travailleur);
        exit(EXIT_FAILURE);
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "(%s) Probleme creation cle\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    /* Recuperation file de message */
    fm = msgget(cle_mecano, 0);
    if (fm == -1) {
	    fprintf(stderr, "(%s) Probleme recuperation file de message\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    return fm;
}