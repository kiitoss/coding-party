#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

int connexion_fm_mecano(char *type_travailleur) {
    key_t cle_mecano;
    FILE *fich_cle;
    int fm;

    /* On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE,"r");
    if (fich_cle == NULL) {
        fprintf(stderr, "(%s) Lancement client impossible\n", type_travailleur);
        exit(EXIT_FAILURE);
    }

    /* Creation de la cle */
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

void deconnexion_fm_mecano(int fm) {
    msgctl(fm, IPC_RMID, NULL);
}


void envoie_requete(int fm, int ordre_exp) {
    requete_t requete;

    /* creation de la requete */
    requete.type = ordre_exp;
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_t), 0);
}


reponse_t attend_reponse(int fm, int ordre_exp) {
    reponse_t reponse;
    int res_rcv;

    /* attente de la reponse */
    res_rcv = msgrcv(fm, &reponse, sizeof(reponse_t), ordre_exp, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return reponse;
}